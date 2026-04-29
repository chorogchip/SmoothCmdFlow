# Small VM Manipulation Domain

Goal: inspect and manipulate a simple VM state with small, typed commands.

This domain is intentionally small: registers, memory addresses, and constants are represented as functions.

## Types

- `Vm`
- `Register`
- `Address`
- `I64`
- `Bool`

## Function Sketch

- global `vm(): Vm`
- argument `zero-reg(): Register`
- argument `one-reg(): Register`
- argument `pc(): Register`
- argument `addr(): Address`
- method `Address.stack(): Address`
- method `Vm.step(): Vm`
- method `Vm.run(I64): Vm`
- method `Vm.set(Register, I64): Bool`
- method `Vm.peek(Address): I64`

## Examples

```
vm.step
vm.run 1
vm.set zero-reg | 0
vm.peek addr.stack
```
