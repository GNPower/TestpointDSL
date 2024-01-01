from __future__ import annotations
import datetime
from typing import Callable, Dict, List

from .values import RuntimeValue, NullValue, NumberValue, BooleanValue, NativeFunctionValue, StringValue


def nativePrint(args: List[RuntimeValue], scope: Scope) -> RuntimeValue:
    print(*args)
    return NullValue()


def timeFunction(args: List[RuntimeValue], scope: Scope) -> RuntimeValue:
    return NumberValue(int(datetime.datetime.utcnow().timestamp()))


def setupScope(scope: Scope) -> None:
    # Constant Values For Comparison
    scope.declareVariable("null", NullValue())
    scope.declareVariable("true", BooleanValue(True))
    scope.declareVariable("false", BooleanValue(False))

    # Non-Constant Varialbes For Reassignment By User
    scope.declareVariable("CONTEXT.NAME", StringValue(["Undefined", "Test", "Name"]), constant = False)
    scope.declareVariable("CONTEXT.AUTHOR", StringValue(["Unknown", "Author"]), constant = False)

    # Builtin Functions
    scope.declareVariable("print", NativeFunctionValue(nativePrint))
    scope.declareVariable("time", NativeFunctionValue(timeFunction))

    # Temp Variables For Testing
    scope.declareVariable("alpha", NumberValue(20))
    scope.declareVariable("t", NumberValue(0.1))
    scope.declareVariable("vA", NumberValue(3.3))


class Scope():
    parent: Scope
    variables: Dict[str, RuntimeValue]
    constants: List[str]


    def __init__(self, parent: Scope = None) -> None:
        self.parent = parent
        self.variables = {}
        self.constants = []
        if not parent:
            setupScope(self)
    

    def __str__(self) -> str:
        parent = self.parent.__str__().replace("\n", "\n\t\t")
        string = f"Scope(\n\tparent: {parent},\n\tvariables: {self.variables.keys()}\n\tconstants: {self.constants}"
        return string
    

    def resolve(self, name: str) -> Scope:
        if name in self.variables:
            return self
        if not self.parent:
            raise ValueError(f"Cannot resolve non-existent variable named {name}")
        return self.parent.resolve(name)


    def declareVariable(self, name: str, value: RuntimeValue, constant: bool = True) -> RuntimeValue:
        if name in self.variables:
            raise ValueError(f"{name} is already defined. Assign it instead")
        self.variables[name] = value
        if constant:
            self.constants.append(name)
        return value
    

    def assignVariable(self, name: str, value: RuntimeValue) -> RuntimeValue:
        scope = self.resolve(name)
        if name in scope.constants:
            raise ValueError(f"Cannot assign a new value to constant named {name}")
        scope.variables[name] = value
        return value
    

    def getVariable(self, name: str) -> RuntimeValue:
        scope = self.resolve(name)
        return scope.variables[name]


FunctionCall = Callable[[List[RuntimeValue], Scope], RuntimeValue]