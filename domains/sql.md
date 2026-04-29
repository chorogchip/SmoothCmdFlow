# SQL Domain

Goal: explore and compose small SQL-like queries without string literals.

Names that would normally be strings are modeled as functions. For example, `users`, `age`, and `alice.name` are functions that return typed values.

## Types

- `Table`
- `Column`
- `Predicate`
- `Rows`
- `Scalar`

## Function Sketch

- global `users(): Table`
- global `orders(): Table`
- argument `age(): Column`
- argument `name(): Column`
- argument `age-gt(I64): Predicate`
- argument `name-is(Scalar): Predicate`
- global `alice(): Row`
- method `Row.name(): Scalar`
- method `Table.where(Predicate): Table`
- method `Table.select(Column): Rows`

## Examples

```
users.where age-gt 18 || select name
users.where name-is alice.name || select age
orders.select name
```
