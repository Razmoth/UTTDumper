# UTTDumper
A tool to help with Unity type trees
_____________________________________________________________________________________________________________________________
How to use:
 - Update `config.toml` with needed values (more details below) and inject it into target process.

```
#	- Update [engine].
#	- Define new key [<name>].
#	- Update values of fields.
#	- Update name in [engine].

[engine]
name = "dummy"

[dummy]
delay = 0 # seconds to wait before dumping.
binary = "UnityPlayer.dll" # name of binary to dump from.
output_dir = "" # output folder (defaults to "cwd/<name>|data/data/<pkg_name>/<name>" if empty).
transfer = 265 # TransferInstruction flags (check transfer.h for possible values).
json_dump = true # Enable json output (classes.json/info.json).
text_dump = true #	Enable text files output (RTTI.dump/structs.dump).
binary_dump = true # Enable binary files output (strings.dat/structs.dat).
version = 0xF07E30 # GetUnityBuildFullVersion RVA or string (ex '2017.3.4f1').
common_strings_begin = 0x167E970 # Unity::CommonString::BufferBegin RVA.
common_strings_end = 0x167E978 # Unity::CommonString::BufferEnd RVA.
rtti = 0x19E0A00 # RTTI::ms_runtimeTypes RVA.
type_tree_ctor = 0x90FB90 # TypeTree::TypeTree RVA.
type_tree = 0x9221E0 # TypeTreeCache::GetTypeTree or GenerateTypeTree RVA (TypeTreeCache::GetTypeTree: version > 2019.x.x else GenerateTypeTree).
produce = 0x58E370 # Object::Produce RVA.
```
_____________________________________________________________________________________________________________________________
Special Thank to:
- [DaZombieKiller](https://github.com/DaZombieKiller)
- [ds5678](https://github.com/ds5678)
- [Khang06](https://github.com/Khang06)
- [Modder4869](https://github.com/Modder4869)
- [Ladilib](https://github.com/ladilib/)
- [Dimbreath](https://github.com/Dimbreath)
_____________________________________________________________________________________________________________________________

If you find `UTTDumper` useful, you can leave a star 😄
Thank you, looking forward for your feedback
