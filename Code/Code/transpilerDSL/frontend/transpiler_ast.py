from __future__ import annotations
from dataclasses import dataclass
from enum import Enum
from typing import List


class NodeType(Enum):
    # Statements (0 -> 999)
    Program             = 0
    VaribaleDeclaration = 1
    FunctionDeclaration = 2

    # Expressions (1000 -> 1999)
    AssignmentExpr      = 1000
    MemberExpr          = 1001
    CallExpr            = 1002

    # Literals (2000 -> 2999)
    Property            = 2000
    ObjectLiteral       = 2001
    FunctionLiteral     = 2002
    NumericLiteral      = 2003
    NumericArrayLiteral = 2004
    StringArrayLiteral  = 2005
    Identifier          = 2006
    ContextIdentifier   = 2007
    BinaryExpr          = 2008
    ParenthesizedExpr   = 2009


class Statement:
    kind: NodeType

    def __init__(self, kind: NodeType) -> None:
        self.kind = kind


class Program(Statement):
    body: List[Statement]

    def __init__(self) -> None:
        super().__init__(NodeType.Program)
        self.body = []

    def __str__(self) -> str:
        string = f"Program(\n\tbody: [\n"
        for statement in self.body:
            string += f"\t\t" + statement.__str__().replace("\n", "\n\t\t") + ",\n"
        string += "\t]\n)"
        return string


class VaribaleDeclaration(Statement):
    constant: bool
    identifier: str
    value: Expression

    def __init__(self, identifier: str, value: Expression = None, constant: bool = True) -> None:
        super().__init__(NodeType.VaribaleDeclaration)
        self.identifier = identifier
        self.value = value
        self.constant = constant
    
    def __str__(self) -> str:
        value = self.value.__str__().replace("\n", "\n\t")
        return f"VaribaleDeclaration(\n\tidentifier: {self.identifier},\n\tconstant: {self.constant},\n\tvalue: {value}\n)"


class FunctionDeclaration(Statement):
    parameters: List[str]
    name: str
    body: List[Statement]

    def __init__(self, parameters: List[str], name: str, body: Statement) -> None:
        super().__init__(NodeType.FunctionDeclaration)
        self.parameters = parameters
        self.name = name
        self.body = body

    def __str__(self) -> str:
        string = f"FunctionDeclaration(\n\tparameters: {self.parameters},\n\tname: {self.name},\n\tbody: [\n"
        for statement in self.body:
            string += f"\t\t" + statement.__str__().replace("\n", "\n\t\t") + ",\n"
        string += "\t]\n)"
        return string


class Expression(Statement):
    pass


class BinaryExpression(Expression):
    left: Expression
    right: Expression
    operator: str

    def __init__(self, left: Expression, right: Expression, operator: str) -> None:
        super().__init__(NodeType.BinaryExpr)
        self.left = left
        self.right = right
        self.operator = operator

    def __str__(self) -> str:
        left_str = self.left.__str__().replace("\n", "\n\t")
        right_str = self.right.__str__().replace("\n", "\n\t")
        operator_str = self.operator.__str__().replace("\n", "\n\t")
        return f"BinaryExpression(\n\tleft: {left_str},\n\tright: {right_str},\n\toperator: {operator_str}\n)"


class MemberExpression(Expression):
    object: Expression
    property: Expression
    computed: bool

    def __init__(self, object: Expression, property: Expression, computed: bool) -> None:
        super().__init__(NodeType.MemberExpr)
        self.object = object
        self.property = property
        self.computed = computed

    def __str__(self) -> str:
        object = self.object.__str__().replace("\n", "\n\t")
        property = self.property.__str__().replace("\n", "\n\t")
        return f"MemberExpression(\n\tobject: {object},\n\tproperty: {property},\n\tcomputed: {self.computed}\n)"


class CallExpression(Expression):
    args: List[Expression]
    callee: Expression

    def __init__(self, args: List[Expression], callee: Expression) -> None:
        super().__init__(NodeType.CallExpr)
        self.args = args
        self.callee = callee
    
    def __str__(self) -> str:
        string = f"CallExpression(\n\targs: [\n"
        for argument in self.args:
            string += f"\t\t" + argument.__str__().replace("\n", "\n\t\t") + ",\n"
        callee = self.callee.__str__().replace("\n", "\n\t")
        string += f"\t]\n\tcallee: {callee}\n)"
        return string


class AssignmentExpression(Expression):
    assignee: Expression
    value: Expression

    def __init__(self, assignee: Expression, value: Expression) -> None:
        super().__init__(NodeType.AssignmentExpr)
        self.assignee = assignee
        self.value = value
    
    def __str__(self) -> str:
        assignee = self.assignee.__str__().replace("\n", "\n\t")
        value = self.value.__str__().replace("\n", "\n\t")
        return f"AssignmentExpression(\n\tassignee: {assignee},\n\tvalue: {value}\n)"


class Identifier(Expression):
    symbol: str

    def __init__(self, symbol: str) -> None:
        super().__init__(NodeType.Identifier)
        self.symbol = symbol

    def __str__(self) -> str:
        return f"Identifier( symbol: \"{self.symbol}\" )"


class ContextIdentifier(Expression):
    symbol: str

    def __init__(self, symbol: str) -> None:
        super().__init__(NodeType.ContextIdentifier)
        self.symbol = symbol

    def __str__(self) -> str:
        return f"ContextIdentifier( symbol: \"{self.symbol}\" )"


class NumericLiteral(Expression):
    value: float

    def __init__(self, value: float) -> None:
        super().__init__(NodeType.NumericLiteral)
        self.value = value

    def __str__(self) -> str:
        return f"NumericLiteral( value: {self.value} )"


class NumericArrayLiteral(Expression):
    value: List[float]

    def __init__(self, value: List[float]) -> None:
        super().__init__(NodeType.NumericArrayLiteral)
        self.value = value

    def __str__(self) -> str:
        return f"NumericArrayLiteral( value: {self.value} )"
    

class StringArrayLiteral(Expression):
    value: List[str]

    def __init__(self, value: List[str]) -> None:
        super().__init__(NodeType.NumericArrayLiteral)
        self.value = value

    def __str__(self) -> str:
        return f"StringArrayLiteral( value: {self.value} )"


class Property(Expression):
    key: str
    value: Expression

    def __init__(self, key: str, value: Expression) -> None:
        super().__init__(NodeType.Property)
        self.key = key
        self.value = value

    def __str__(self) -> str:
        value = self.value.__str__().replace("\n", "\n\t")
        return f"Property(\n\tkey: {self.key},\n\tvalue: {value}\n)"


class ObjectLiteral(Expression):
    properties: List[Property]

    def __init__(self, properties: List[Property]) -> None:
        super().__init__(NodeType.ObjectLiteral)
        self.properties = properties
    
    def __str__(self) -> str:
        string = f"ObjectLiteral(\n\tproperties: [\n"
        for property in self.properties:
            string += f"\t\t" + property.__str__().replace("\n", "\n\t\t") + ",\n"
        string += "\t]\n)"
        return string


class FunctionLiteral(Expression):
    function: Identifier
    parameters: List[Property]

    def __init__(self, function: Identifier, parameters: List[Property]) -> None:
        super().__init__(NodeType.FunctionLiteral)
        self.function = function
        self.parameters = parameters
    
    def __str__(self) -> str:
        string = f"FunctionLiteral(\n\tfunction: {self.function}\n\tparameters: [\n"
        for param in self.parameters:
            string += f"\t\t" + param.__str__().replace("\n", "\n\t\t") + ",\n"
        string += "\t]\n)"
        return string


class ParenthesizedExpression(Expression):
    inside: Expression

    def __init__(self, inside: Expression) -> None:
        super().__init__(NodeType.ParenthesizedExpr)
        self.inside = inside

    def __str__(self) -> str:
        inside = self.inside.__str__().replace("\n", "\n\t")
        return f"ParenthesizedExpression(\n\tinside: {inside}\n)"