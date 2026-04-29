# Linux File System Domain

Goal: browse and perform small file-system operations with typed paths and options.

Path segments are functions instead of string literals. For example, `home.user.project` returns a path-like value.

## Types

- `Path`
- `Entry`
- `Entries`
- `Pattern`
- `Bool`

## Function Sketch

- global `home(): Path`
- method `Path.user(): Path`
- method `Path.project(): Path`
- method `Path.ls(): Entries`
- method `Path.touch(Path): Bool`
- method `Path.rm(Path): Bool`
- argument `readme(): Path`
- method `Path.md(): Path`
- argument `todo(): Path`
- method `Path.md(): Path`
- argument `cpp(): Pattern`
- method `Pattern.files(): Pattern`
- method `Entries.filter(Pattern): Entries`

## Examples

```
home.user.project.ls
home.user.project.ls.filter cpp.files
home.user.project.touch readme.md
home.user.project.rm todo.md
```
