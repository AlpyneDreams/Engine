from functools import cache
from clang.cindex import Index, CursorKind, CompilationDatabase, Cursor
import os, os.path, sys, re, string

# Usage: rtti.py <input.cpp> [output.meta.cpp]
# Generates a single meta file for a single translation unit
# Invoked by src/meson.build

SRC_DIR = '../src'
ARGS = [
    '-xc++', # allows inspecting headers
    '-std=c++20'
]

# https://ansi.gabebanks.net/
class Colors:
    DIM = '\033[2m'
    END = '\033[0m'

# Read arguments: <input> [output]
infile = sys.argv[1]
outfile = sys.argv[2] if len(sys.argv) > 2 else infile + '.meta.cpp'

# Set to false when we've written any metadata
discard = True

out = ""

# Converts a property name to a fancy human-friendly name
@cache
def display_name(name):
    name = name.replace('_', ' ')

    # Complex conversion of camelCase to spaced words
    # (same pattern as converting to snake_case)
    name = re.sub(r'(.)([A-Z][a-z]+)', r'\1 \2', name)
    name = re.sub(r'([a-z0-9])([A-Z])', r'\1 \2', name)

    # Capitalize first letter of each word
    # (advanced titlecase algo from python docs)
    return re.sub(r"[A-Za-z]+('[A-Za-z]+)?",
        lambda m: m.group(0)[0].upper() + m.group(0)[1:], name)

def out_write(*args, indent=0):
    global out
    if not args:
        out += '\n'
    out += ' ' * indent * 4
    out += ''.join([*args]) + '\n'

write = out_write

def is_subpath(path, prefix):
    from os.path import normpath
    return normpath(path).startswith(normpath(prefix))

def in_path(nodes, path):
    return [n for n in nodes if is_subpath(n.location.file.name, path)]

def with_kind(nodes, kinds: list[CursorKind]):
    return [n for n in nodes if n.kind in kinds]

# Check if class extends Reflect
def is_reflectable(clazz):
    # Don't reflect the Reflect class itself
    if clazz.spelling.endswith('Reflect'):
        return False
    
    # Check if this class has a reflectable base class
    # i.e. a class that extends Reflect
    for c in clazz.get_definition().get_children():
        if c.kind == CursorKind.CXX_BASE_SPECIFIER:
            if c.spelling.endswith('Reflect'):
                return True
            elif is_reflectable(c):
                return True
    
    return False

# Traverse the AST and find all reflected classes
def traverse(nodes: list[Cursor], parent=None, ident=0):
    includes = []

    # Override write to add extra indent
    write = lambda *args, indent=0: out_write(*args, indent=(indent+ident))

    def write_class(node, prefix=''):
        includes = []
        global discard
        discard = False
        keyword = 'struct' if node.kind == CursorKind.STRUCT_DECL else 'class'

        # Get all fields, methods, etc.
        fields = []
        classes = []
        for n in node.get_children():
            match n.kind:
                case CursorKind.CXX_BASE_SPECIFIER:
                    pass # TODO: base class
                case CursorKind.FIELD_DECL:
                    fields += [n]
                case CursorKind.CXX_METHOD:
                    pass # TODO: class methods
                case CursorKind.CLASS_DECL | CursorKind.STRUCT_DECL:
                    classes += [n]
                case default:
                    # TODO: other stuff
                    #print(n.spelling, n.kind)
                    pass
        
        # Source file location of class
        file = f'{os.path.relpath(node.location.file.name, SRC_DIR)}'
        location = f'{file}:{node.location.line}'

        includes += [os.path.normpath(file)]

        # Skip anonymous classes
        if node.is_anonymous():
            print('Skipping anonymous struct:', location)
            return
        
        # Begin class RTTI definition
        name = f'{keyword} {prefix}{node.displayname}'
        write(f'// {location}')
        write('template <>')
        write(f'Class RTTI<{name}> = {{')

        # Write name, size
        write(f'.name = "{node.displayname}",', indent=1)
        write(f'.displayName = "{display_name(node.displayname)}",', indent=1)
        write(f'.size = sizeof({name}),', indent=1)

        # Write fields
        if len(fields) > 0:
            write('.fields = {', indent=1)
            for n in fields:
                typeclass = n.type.get_canonical()
                write(f'{{ "{n.spelling}", "{display_name(n.spelling)}", TypeID<{typeclass.spelling}>(), offsetof({name}, {n.spelling}) }},', indent=2)
            write('},', indent=1)

        # End class RTTI
        write('};\n')

        # Write nested classes too
        for c in classes:
            includes += write_class(c, node.displayname + '::')
        
        return includes
    
    for n in nodes:
        name = n.displayname
        match n.kind:
            case CursorKind.NAMESPACE:
                # TODO: namespace handling
                #write(f'namespace {n.spelling}::_rtti {{')
                includes += traverse(n.get_children(), parent=n, ident=ident)
                #write('}')
            
            case CursorKind.CLASS_DECL | CursorKind.STRUCT_DECL:
                
                if n.is_definition() and is_reflectable(n):
                    print('Reflecting class:', n.spelling)
                    #tokens = n.get_tokens()
                    #print([t.spelling for t in tokens])
                    includes += write_class(n)
            
            case default:
                # TODO: other stuff
                #print(name, n.kind)
                pass
    return set(includes)

def main():
    global out
    # Read compile_commands.json
    compile_commands = CompilationDatabase.fromDirectory('../build')

    index = Index.create()

    # Get compile commands for this specific file
    cmd = compile_commands.getCompileCommands(infile)[0]

    # Get include dirs
    args = [arg for arg in cmd.arguments if arg.startswith('-I')]
    # Add default args
    args += ARGS
    # Parse translation unit
    tu = index.parse(cmd.filename, args)

    # Print diagnostics
    for msg in tu.diagnostics:
        print(Colors.DIM + str(msg) + Colors.END)

    # Only inspect files inside the project source directory
    nodes = in_path(tu.cursor.get_children(), SRC_DIR)

    # Meta file header
    write('// Autogenerated by tools/rtti.py')
    write('#include "common/Reflection.h"')
    write()

    # TODO: Improve header crap!!
    # Add includes from this translation unit
    includes_pos = len(out)
    '''
    includes = set()
    for i in tu.get_includes():
        path = os.path.normpath(i.location.file.name)
        if path.endswith('.h') and is_subpath(path, SRC_DIR):
            if path in includes:
                continue
            write(f'#include "{path}"')
            includes.add(path)
    '''

    # Begin RTTI namesapce
    write()
    write('namespace engine::rtti')
    write('{')

    # Traverse the AST and output reflection data
    includes = traverse(nodes, ident=1)

    # End RTTI namespace
    write('}')

    includes = '\n'.join([f'#include "{i}"' for i in includes])
    out = out[:includes_pos] + includes + '\n' + out[includes_pos:]

    output = open(outfile, 'w')
    # Blank out meta files that are empty
    if discard:
        output.write('// Autogenerated by tools/rtti.py\n')
        output.write('// This file intentionally left blank\n')
    else:
        output.write(out)

    # Done
    output.close()

if __name__ == '__main__':
    main()