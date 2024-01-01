import sys
sys.path.append("..") # Adds higher directory to python modules path.

from .values import NullValue, NumberValue, RuntimeValue, ValueType, ObjectValue, FunctionValue, StringValue, ArrayValue
from .scope import Scope
from frontend.transpiler_ast import BinaryExpression, NumericLiteral, Program, Statement, NodeType, Identifier, VaribaleDeclaration, AssignmentExpression, ObjectLiteral, CallExpression, FunctionDeclaration


def evaluateProgram(program: Program, scope: Scope) -> RuntimeValue:
    lastEvaluated = NullValue()

    for statement in program.body:
        lastEvaluated = evaluate(statement, scope)

    return lastEvaluated


def evaluateVariableDeclaration(declaration: VaribaleDeclaration, scope: Scope) -> RuntimeValue:
    value = evaluate(declaration.value, scope) if declaration.value else NullValue()
    return scope.declareVariable(declaration.identifier, value, declaration.constant)


def evaluateNumericBinaryExpression(leftSide: NumberValue, rightSide: NumberValue, operator: str) -> NumberValue:
    match operator:
        case "+":
            result = float(leftSide.value) + float(rightSide.value)
        case "-":
            result = float(leftSide.value) - float(rightSide.value)
        case "*":
            result = float(leftSide.value) * float(rightSide.value)
        case "/":
            result = float(leftSide.value) / float(rightSide.value)
        case "%":
            result = float(leftSide.value) % float(rightSide.value)
        case _:
            result = float(0)
    return NumberValue(result)


def evaluateBinaryExpression(binop: BinaryExpression, scope: Scope) -> RuntimeError:
    leftSide = evaluate(binop.left, scope)
    rightSide = evaluate(binop.right, scope)

    if (leftSide.type == ValueType.Number and rightSide.type == ValueType.Number):
        return evaluateNumericBinaryExpression(leftSide, rightSide, binop.operator)
    return NullValue()


def evaluateIdentifier(identifier: Identifier, scope: Scope) -> RuntimeValue:
    return scope.getVariable(identifier.symbol)


def evaluateAssignment(node: AssignmentExpression, scope: Scope) -> RuntimeValue:
    if node.assignee.kind == NodeType.ContextIdentifier or node.assignee.kind == NodeType.ContextIdentifier:
        return scope.assignVariable(node.assignee.symbol, evaluate(node.value, scope))
    raise ValueError(f"Cannot assign to literals, only identifiers supported on LHS\n{node.assignee}")
    

def evaluate(astNode: Statement, scope: Scope) -> RuntimeValue:
    match astNode.kind:
        case NodeType.NumericLiteral:
            return NumberValue(astNode.value)
        case NodeType.StringArrayLiteral:
            return StringValue(astNode.value)
        case NodeType.NumericArrayLiteral:
            return ArrayValue(astNode.value)
        case NodeType.Identifier:
            return evaluateIdentifier(astNode, scope)    
        case NodeType.AssignmentExpr:
            return evaluateAssignment(astNode, scope)
        case NodeType.BinaryExpr:
            return evaluateBinaryExpression(astNode, scope)
        case NodeType.Program:
            return evaluateProgram(astNode, scope)
        case NodeType.VaribaleDeclaration:
            return evaluateVariableDeclaration(astNode, scope)
        case _:
            print(f"AST node type not currently supported: {astNode}")
            raise SystemExit("Interpreter cannot continue due to above errors")