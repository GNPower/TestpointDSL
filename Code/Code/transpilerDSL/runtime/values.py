from __future__ import annotations
from enum import Enum
from typing import Callable, Dict, List
import traceback
import sys
sys.path.append("..") # Adds higher directory to python modules path.

from frontend.transpiler_ast import Statement


class ValueType(Enum):
    Null                = 1
    Number              = 2
    Array               = 3
    String              = 4
    Boolean             = 5
    Object              = 6
    NativeFunction      = 7
    Function            = 8


class RuntimeValue:
    type: ValueType

    def __init__(self, type: ValueType) -> None:
        self.type = type


class NullValue(RuntimeValue):
    value: str

    def __init__(self) -> None:
        super().__init__(ValueType.Null)
        self.value = None

    def __str__(self) -> str:
        return f"NullValue( value: {self.value} )"


class NumberValue(RuntimeValue):
    value: float

    def __init__(self, value: float) -> None:
        super().__init__(ValueType.Number)
        self.value = value
    
    def __str__(self) -> str:
        return f"NumberValue( value: \"{self.value}\" )"


class StringValue(RuntimeValue):
    value: List[str]

    def __init__(self, value: List[str]) -> None:
        super().__init__(ValueType.String)
        self.value = value
    
    def __str__(self) -> str:
        return f"StringValue( value: \"{self.value}\" )"


class ArrayValue(RuntimeValue):
    value: List[float]

    def __init__(self, value: float) -> None:
        super().__init__(ValueType.Array)
        self.value = value
    
    def __str__(self) -> str:
        return f"ArrayValue( value: \"{self.value}\" )"


class BooleanValue(RuntimeValue):
    value: bool

    def __init__(self, value: bool) -> None:
        super().__init__(ValueType.Boolean)
        self.value = value
    
    def __str__(self) -> str:
        return f"BooleanValue( value: \"{self.value}\" )"


class ObjectValue(RuntimeValue):
    properties: Dict[str, RuntimeValue]

    def __init__(self, properties: Dict[str, RuntimeValue] = {}) -> None:
        super().__init__(ValueType.Object)
        self.properties = properties

    def __str__(self) -> str:
        string = f"ObjectValue(\n\tproperties: {{\n"
        for i in self.properties:
            string += f"\t\t{i}: " + self.properties[i].__str__().replace("\n", "\n\t\t") + ",\n"
        string += "\t}\n)"
        return string


class NativeFunctionValue(RuntimeValue):

    def __init__(self, call) -> None:
        super().__init__(ValueType.NativeFunction)
        self.call = call

    def __str__(self) -> str:
        return f"NativeFunctionValue( call: \"{self.call}\" )"


class FunctionValue(RuntimeValue):
    name: str
    parameters: List[str]
    declarationScope: None
    body: List[Statement]

    def __init__(self, name: str, parameters: List[str], declarationScope, body: List[Statement]) -> None:
        super().__init__(ValueType.Function)
        self.name = name
        self.parameters = parameters
        self.declarationScope = declarationScope
        self.body = body

    def __str__(self) -> str:
        declarationScope = self.declarationScope.__str__().replace("\n", "\n\t\t")
        string = f"FunctionDeclaration(\n\tname: {self.name},\n\tparameters: {self.parameters},\n\tdeclarationScope: {declarationScope},\n\tbody: [\n"
        for statement in self.body:
            string += f"\t\t" + statement.__str__().replace("\n", "\n\t\t") + ",\n"
        string += "\t]\n)"
        return string
