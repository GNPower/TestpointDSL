from .transpiler_lexer import TokenType


def FnAssert():
    pass


def FnSet():
    pass


SupportedNativeFunctions = {
    TokenType.ASSERT: {
        "call": FnAssert,
        "parameters": [
            {
                "name": "weak",
                "type": [TokenType.WEAK],
                "optional": True,
            },
            {
                "name": "input",
                "type": [TokenType.IDENTIFIER],
                "optional": False,
            },
            {
                "name": "comparison",
                "type": [TokenType.EQUAL, TokenType.MATCHES],
                "optional": False,
            },
            {
                "name": "golden",
                "type": [TokenType.IDENTIFIER],
                "optional": False,
            },
            {
                "name": "golen_unit",
                "type": [TokenType.UNIT_MILAMPS, TokenType.UNIT_MILSECS, TokenType.UNIT_PERCENT, TokenType.UNIT_VOLTS],
                "optional": True,
            },
            {
                "name": "tolerance",
                "type": [TokenType.TOLERANCE],
                "optional": True,
            },
            {
                "name": "tolerance_val",
                "type": [TokenType.NUMBER],
                "optional": True,
            },
            {
                "name": "tolerance_unit",
                "type": [TokenType.UNIT_MILAMPS, TokenType.UNIT_MILSECS, TokenType.UNIT_PERCENT, TokenType.UNIT_VOLTS],
                "optional": True,
            }
        ]
    },
    TokenType.SET: {
        "call": FnSet,
        "parameters": [
            {
                "name": "input",
                "type": [TokenType.IDENTIFIER],
                "optional": False,
            },
            {
                "name": "value",
                "type": [TokenType.LOW, TokenType.HIGH],
                "optional": False,
            }
        ]
    },
    TokenType.DELAY: {

    },
    TokenType.RECORD: {

    },
}