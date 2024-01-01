from typing import List
from .transpiler_ast import ContextIdentifier, NodeType, Statement, Program, Expression, BinaryExpression, NumericLiteral, Identifier, ParenthesizedExpression, StringArrayLiteral, VaribaleDeclaration, AssignmentExpression, ObjectLiteral, Property, CallExpression, MemberExpression, FunctionDeclaration, FunctionLiteral
from .transpiler_lexer import Lexer, Token, TokenType
from .transpiler_objects import SupportedHardwareObjects
from .transpiler_functions import SupportedNativeFunctions


class Parser():
    tokens: List[Token] = []
    lexer: Lexer
    program: Program


    def __init__(self) -> None:
        self.lexer = Lexer()


    def __not_eof(self) -> bool:
        return self.tokens[0].type != TokenType.EOF
    

    def __at(self) -> Token:
        return self.tokens[0]
    

    def __eat(self, expectedValue: TokenType = None, errorMessage: str = "") -> Token:
        if expectedValue:
            value = self.tokens.pop(0)
            if not value or value.type != expectedValue:
                print(f"Error on: {value}\nExpecting: {expectedValue}\n\n{errorMessage}")
                raise SystemExit('Compiler cannot continue due to above errors')
            return value
        else:
            return self.tokens.pop(0)


    def __parse_builtin_expression_context(self) -> Expression:
        currentToken = self.__at().type

        match currentToken:
            case TokenType.NAME:
                self.__eat() # Eat the name token
                return ContextIdentifier("CONTEXT.NAME")
            case TokenType.AUTHOR:
                self.__eat() # eat the author token
                return ContextIdentifier("CONTEXT.AUTHOR")
            case _:
                raise ValueError(f"Error: Unexprected token found during parsing: {self.__at()}")


    def __parse_builtin_expression(self) -> Expression:
        currentToken = self.__at().type

        match currentToken:
            case TokenType.CONTEXT:                
                self.__eat() # Eat the context token
                return self.__parse_builtin_expression_context()
            case _:
                raise ValueError(f"Error: Unexprected token found during parsing: {self.__at()}")


    def __parse_primary_expression(self) -> Expression:
        currentToken = self.__at().type

        match currentToken:
            case TokenType.IDENTIFIER:
                return Identifier(self.__eat().value)
            case TokenType.NUMBER:
                return NumericLiteral(float(self.__eat().value))
            case TokenType.OPENPAREN:
                self.__eat()
                value = self.__parse_expression()
                self.__eat(TokenType.CLOSEPAREN, "Missing closing parenthesis somewhere")
                return ParenthesizedExpression(value)
            case _:
                return self.__parse_builtin_expression()


    def __parse_multiplicative_expression(self) -> Expression:
        leftSide = self.__parse_primary_expression()

        while(self.__at().value == "*" or self.__at().value == "/" or self.__at().value == "%"):
            operator = self.__eat().value
            rightSide = self.__parse_primary_expression()
            leftSide = BinaryExpression(leftSide, rightSide, operator)

        return leftSide


    def __parse_additive_expression(self) -> Expression:
        leftSide = self.__parse_multiplicative_expression()

        while(self.__at().value == "+" or self.__at().value == "-"):
            operator = self.__eat().value
            rightSide = self.__parse_multiplicative_expression()
            leftSide = BinaryExpression(leftSide, rightSide, operator)

        return leftSide
    

    def __parse_object_expression(self) -> Expression:
        self.__eat() # Eat hardware token
        
        # Check we have a valid hardware object
        if not self.__at().type in SupportedHardwareObjects.keys():
            raise ValueError(f"Hardware object {self.__at()} not supported")
        hardware = self.__eat()
        # Get the name
        self.__eat(TokenType.NAMED, f"Hardware object {hardware} must start with NAME keyword")
        hardware_name = self.__eat(TokenType.IDENTIFIER, f"Hardware object {hardware} NAME must be followed with Identifier")
        self.__eat(TokenType.ON, f"Hardware object {hardware} ON keyword must follow NAME")
        hardware_on_type = self.__eat(TokenType.IDENTIFIER, f"Hardware object {hardware} ON must be followed with port type Identifier")
        hardware_on_port = self.__eat(TokenType.NUMBER, f"Hardware object {hardware} ON port type must be followed with port number")
        hardware_using = None
        if self.__at().type == TokenType.USING:
            self.__eat() # Eat using
            hardware_using = self.__eat(TokenType.IDENTIFIER, f"Hardware object {hardware} USING must be followed with Identifier")

        # Store the object
        properties: List[Property] = [
            Property("name", hardware_name),
            Property("on_port", hardware_on_type),
            Property("port_num", hardware_on_port),
        ]
        if hardware_using:
            properties.append(Property("using", hardware_using))


        self.__eat(TokenType.EOL, "Hardware object declarations must end with a new line")
        return ObjectLiteral(properties)


    def __parse_assignment_expression(self) -> Expression:
        # leftSide = self.__parse_object_expression() # TODO: Should this be here??
        leftSide = self.__parse_additive_expression() # Required since __parse_variable_declaration declaration field parses a full expression

        if self.__at().type == TokenType.EQUALS:
            self.__eat() # Eat equals
            value: List[Expression] = []
            while (self.__at().type != TokenType.EOL):
                value.append(self.__parse_additive_expression())
            self.__eat(TokenType.EOL, f"Assignment expression must end with EndOfLine character: {leftSide}")

            match leftSide.kind:
                case NodeType.ContextIdentifier:
                    value = StringArrayLiteral([id.symbol for id in value])                
                                        
            return AssignmentExpression(leftSide, value)
        
        return leftSide


    def __parse_expression(self) -> Expression:
        return self.__parse_assignment_expression()


    def __parse_variable_declaration(self) -> Statement:
        isConstant = self.__eat().type == TokenType.CONSTANT
        identifier = self.__eat(TokenType.IDENTIFIER, "Identifier neame required for variable declarations").value
        
        self.__eat(TokenType.EQUALS, "Expected equals following identifier")
        declaration = VaribaleDeclaration(identifier, value=self.__parse_expression(), constant=isConstant)
        self.__eat(TokenType.EOL, "Variable declarations must end with newline")
        return declaration
    

    def __parse_native_function_call(self) -> Expression:
        func_call = self.__eat()
        function_requirements = SupportedNativeFunctions[func_call.type]
        parameters: List[Expression] = []
        for param in function_requirements["parameters"]:
            print(param)
            if self.__at().type in param["type"]:
                parameters.append(Property(param["name"], self.__parse_expression()))
            elif param["optional"]:
                parameters.append(Property(param["name"], Identifier("null")))
            else:
                raise ValueError(f"Function call {func_call} must include parameter {param}")
        return FunctionLiteral(Identifier(func_call.value), parameters)
    

    def __parse_statement(self) -> Statement:
        print(f"Parsing {self.__at()}")
        match self.__at().type:
            case TokenType.CONSTANT:
                return self.__parse_variable_declaration()
            case TokenType.HARDWARE:
                return self.__parse_object_expression()
            case _ as item if item in list(SupportedNativeFunctions.keys()):
                return self.__parse_native_function_call()
            case _:
                return self.__parse_expression()
    
    
    def produceAST(self, source: str) -> Program:
        self.tokens = self.lexer.Tokenize(source)
        self.program = Program()

        while self.__not_eof():
            statement = self.__parse_statement()
            if statement == -1:
                return Program()
            self.program.body.append(statement)

        return self.program