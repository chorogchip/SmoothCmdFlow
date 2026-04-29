# SmoothCmdFlow
트리 탐색형 명령 DSL입니다.
처음 접하는 환경의 커맨드라인 인터페이스에서 학습 부담이 적은 명령 언어 체계로 고안했습니다.

# Examples
```
players.filter.stepblock.is stone | .damage 10 | .emit spark |
means: players().filter().stepblock().is(stone()).damage(10).emit(spark())

entity.filter.type.is creeper | .filter.distance.lt 5 | .kill
means: entity().filter().type().is(creeper()).filter().distance().lt(5).kill()
```

# Goals
- low memory load - 외워야 할 것을 최소화
    - 현재 커서에 올 수 있는 선택지의 개수를 적게 유지합니다.
- progressive disclosure - 매번 필요한 정보만 노출
    - 사용자는 매번 제공되는 적은 선택지 중 알맞은 것을 선택하며 명령을 입력할 수 있습니다.
- discoverability - 사용 가능한 기능을 자연스럽게 발견
    - 사용 가능 명령이 트리 형태의 자료에 분배되어 사용과 동시에 기능을 탐색할 수 있습니다.
- learnability - 최소한의 문법 요소 유지
    - 배우기 쉽도록 적은 수의 문법으로 이루어져 있습니다. 모든 심볼은 함수 이름입니다.
- feedback - 자동완성과 자세한 에러 메시지
    - 현재 가능한 선택지들을 보여주고 자동완성/에러 메시지 기능을 제공합니다.
- flexible structure - 동적 명령 구조
    - 탐색할 구조가 런타임에 효율적으로 수정이 가능합니다.

# Grammar
```
tokens: <func_id>, '.', '|'
'.': call next function
'|': separates function arguments and closes the current argument slot.
함수는 모든 곳에서 사용 가능한 함수, 특정 맥락에서 사용 가능한 함수, 특정 타입에 대해 사용 가능한 함수가 존재합니다.
함수의 인자는 0개 이상으로 고정됩니다.
2개 이상의 인자를 받는 함수는 `funcname arg1 | arg2 | arg3 | ... argn | .nextfunc` 형태로 작성합니다.
```
# Target Domains
- In Game Command
- Debug Console
- Log/Database Search
- Image/File Edit
