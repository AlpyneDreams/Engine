import os, os.path, sys, json, time

infiles = sys.argv[1:-1]
outfile = sys.argv[-1]

start_time = time.time()

includes = set()
classes = {}

for filename in infiles:
    filename = os.path.splitext(filename)[0] + '.json'
    if not os.path.exists(filename):
        continue
    with open(filename) as file:
        meta = json.load(file)
        classes.update(meta['classes'])
        for inc in meta['includes']:
            includes.add(inc)

output = open(outfile, 'w')
global_indent = 0
write = lambda *args, indent=0: output.write((' ' * (indent+global_indent) * 4) + ''.join([*args]) + '\n')

write('// Autogenerated by tools/rtti.py')
write('#include "common/Reflection.h"')
write()
for inc in includes:
    write(f'#include "{inc}"')

# Begin RTTI namesapce
write()
write('namespace engine::rtti')
write('{')
global_indent += 1

for spelling, data in classes.items():

    write(f'// {data["location"]}')
    write('template <>')
    write(f'Class& RTTI<{spelling}> = Class::Register(Class {{')

    # Write name, size
    write(f'.name = "{data["name"]}",', indent=1)
    write(f'.displayName = "{data["displayName"]}",', indent=1)
    write(f'.type = {data["type"]},', indent=1)
    write(f'.size = {data["size"]},', indent=1)

    fields = data['fields']
    # Write fields
    if len(fields) > 0:
        write('.fields = {', indent=1)
        for n in fields:
            write(f'{{ "{n["name"]}", "{n["displayName"]}", {n["type"]}, {n["offset"]} }},', indent=2)
        write('},', indent=1)
    
    # End class RTTI
    write('});\n')

# End RTTI namespace
global_indent -= 1
write('}')

output.close()
