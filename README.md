# UTTDumper
A tool to help with Unity type trees

Check the tutorial [here](https://gist.github.com/Modder4869/55df4f3f9fa585a0057b9765e7fe4c4f) (Thanks to Modder4869 for the tutorial)
# How to use:
- Update `[engine]`.
- Define new key `[<name>]`.
- Update values of fields.
- Update name in `[engine]`.
# Values:
- `delay`: seconds to wait before dumping.
- `binary`: name of binary to dump from.
- `output_dir`: output folder (defaults to "cwd/<name>|data/data/<pkg_name>/<name>" if empty).
- `transfer`: `TransferInstruction` flags (check transfer.h for possible values).
- `json_dump`: Enable json output (classes.json/info.json).
- `text_dump`: Enable text files output (RTTI.dump/structs.dump).
- `binary_dump`: Enable binary files output (strings.dat/structs.dat).
- `exclude`: List of types to exclude while dumping.
- `version`: `GetUnityBuildFullVersion` RVA or string (ex '2017.3.4f1').
- `common_strings`: `Unity::CommonString` begin RVA.
- `rtti`: `RTTI::ms_runtimeTypes` RVA.
- `type_tree_ctor`: `TypeTree::TypeTree` RVA.
- `type_tree`: `TypeTreeCache::GetTypeTree` RVA for `version > 2019.x.x` else `GenerateTypeTree` RVA.
- `produce`: `Object::Produce` RVA.
# Example:
```
[engine]
name = "dummy"

[dummy]
delay = 0
binary = "UnityPlayer.dll"
output_dir = ""
transfer = 256
json_dump = true
text_dump = true
binary_dump = true
exclude = [1, 2, 3, 4]
version = 0xF07E30
common_strings = 0x167EA90
rtti = 0x19E0A00
type_tree_ctor = 0x90FB90
type_tree = 0x9221E0
produce = 0x58E370
```
# Special Thank to:
- [DaZombieKiller](https://github.com/DaZombieKiller)
- [ds5678](https://github.com/ds5678)
- [Khang06](https://github.com/Khang06)
- [Modder4869](https://github.com/Modder4869)
- [Ladilib](https://github.com/ladilib/)
- [Dimbreath](https://github.com/Dimbreath)

If you find `UTTDumper` useful, you can leave a star 😄
Thank you, looking forward for your feedback
