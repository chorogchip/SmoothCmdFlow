
# expernal specs

## functions
- global functions: 문장의 시작 혹은 인자의 시작에 쓸 수 있는 함수
    각 맥락에서 최대 8개의 선택지로 제한됩니다.
- method function: 특정 타입에 대해 쓸 수 있는 함수
    각 맥락에서 최대 8개의 선택지로 제한됩니다.
- argument function: 특정 타입이 요구되는 인자의 시작에 쓸 수 있는 함수
    각 맥락에서 제한되는 개수가 없습니다.

문장의 시작엔 오직 global function을 사용할 수 있습니다.
특정 타입 뒤엔 오직 method function을 사용할 수 있습니다.
인자의 시작엔 global/argumet function을 사용할 수 있습니다.
함수의 인자 개수는 함수 선언에 의해 0개 이상으로 고정됩니다.
2개 이상의 인자는 각 인자 뒤에 `|`를 붙여 구분합니다.

ex
```
func0.next0
func1 arg1 | .next0
func2 arg1 | arg2 | .next0
funcn arg1 | arg2 | arg3 | .next0
```

# internal specs

## types
- default types

primitive type은 client에게 `SmoothCmdFlowSetup`이 전달되기 전에 기본 타입으로 자동 등록됩니다.
`SmoothCmdFlowSetup`에는 default type만 등록합니다.
함수의 입출력 타입에서는 등록된 default type `T`와 `Array<T>`를 모두 사용할 수 있습니다.
`Array<T>`는 별도 이름을 가진 타입으로 등록하지 않습니다.
Array value는 method 함수를 직접 가지지 않습니다. 이는 한 상황에 선택 가능한 함수의 개수를 제한하기 위함입니다.
Array value는 원본 타입을 가지고, 원본 타입에 대한 함수를 호출할 수 있습니다.
ex
global function entity(): type Entity
global function entities(): type Array<Entity> (Entity만 setup에 등록)
method function Entity.name(): type String 이 존재한다면
entity.name, entities.name 둘 다 가능

# BNF

이 BNF는 concrete syntax를 표현합니다.
global/method/argument function 사용 가능 여부와 각 함수의 정확한 인자 개수는 semantic validation에서 함수 선언을 기준으로 검사합니다.

```
<S>             ::= <expr>

<expr>          ::= <call> <method_tail>
<method_tail>   ::= "." <call> <method_tail>
                  | ε

<call>          ::= <func0>
                  | <func1>
                  | <func2>
                  | <funcn>

<func0>         ::= <id0>
<func1>         ::= <id1> <expr> "|"
<func2>         ::= <id2> <expr> "|" <expr> "|"
<funcn>         ::= <idn> <expr> "|" <expr> "|" <expr> "|" <more_args>

<more_args>     ::= <expr> "|" <more_args>
                  | ε

<id0>           ::= entity | filter | stepblock | distance | creeper | type | stone | lightning | 10 | kill | origin | player
<id1>           ::= is | summon | emit | damage | lt
<id2>           ::= <function id whose declared arity is 2>
<idn>           ::= <function id whose declared arity is 3 or more>
```

Examples:

```
damage 10 | .emit spark |
setblock pos 13 | 20 | 50 | block stone |
range from | to | step | .map effect |
```
