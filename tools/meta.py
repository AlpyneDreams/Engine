from functools import cache
from clang.cindex import Index, CursorKind, CompilationDatabase, Cursor, AccessSpecifier
import os, os.path, sys, re, json

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
dummy = sys.argv[2] if len(sys.argv) > 2 else infile + '.meta.cpp'
outfile = sys.argv[2] if len(sys.argv) > 2 else infile + '.meta.json'

out_classes = {}

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

    def write_class(node, prefix=''):
        includes = []
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
        out_classes[name] = {
            'name': node.displayname,
            'displayName': display_name(node.displayname),
            'location': location,
            'type': f'TypeID<{name}>()',
            'size': f'sizeof({name})',
            'fields': []
        }

        # Write fields
        def field(n):
            typeclass = n.type.get_canonical()
            return {
                'name': n.spelling,
                'displayName': display_name(n.spelling),
                'type': f'TypeID<{typeclass.spelling}>()',
                'offset': f'offsetof({name}, {n.spelling})',
            }
        
        # Only public fields for now
        out_classes[name]['fields'] += [
            field(n) for n in fields if n.access_specifier == AccessSpecifier.PUBLIC
        ]
        
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

    # Traverse the AST and output reflection data
    includes = traverse(nodes, ident=1)

    out = {}
    out['includes'] = list(includes)
    out['classes'] = out_classes

    output = open(outfile, 'w')
    output.write(json.dumps(out, indent=4))
    output.close()

if __name__ == '__main__':
    main()
