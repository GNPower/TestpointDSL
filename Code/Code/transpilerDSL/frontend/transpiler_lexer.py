from dataclasses import dataclass
from enum import Enum
import re
import json
from typing import List, Optional


class TokenType(Enum):
    # Literal Types (0 -> 999)
    NUMBER      = 0
    IDENTIFIER  = 1

    # Keyword Types (1000 -> 1999)
    WITH        = 1000
    ON          = 1001
    CONSTANT    = 1002
    MATHFUNC    = 1003
    SET         = 1004
    LABEL       = 1005
    ALWAYS      = 1006
    ASSERT      = 1007
    RECORD      = 1008
    DELAY       = 1009    

    # Keyword: WITH Subkeywords (1100 -> 1199)    
    CONTEXT     = 1101
    HARDWARE    = 1102

    # Keyword: CONTEXT Subkeywords (1110 -> 1119)
    NAME        = 1110
    AUTHOR      = 1119

    # Keyword: HARDWARE Subkeywords (1120 -> 1129)
    GPO         = 1120
    GPI         = 1121
    NAMED       = 1122
    USING       = 1123

    # Keyword: ON Subkeywords (1200 -> 1299)
    FAILURE     = 1200
    COMPLETE    = 1201

    # Keyword: SET Subkeywords (1300 -> 1399)
    LOW         = 1300
    HIGH        = 1301

    # Keyword: ALWAYS Subkeywords (1400 -> 1499)
    AT          = 1400

    # Keyword: ASSERT Subkeywords (1500 -> 1599)
    WEAK        = 1500
    EQUAL       = 1501
    MATCHES     = 1502
    GREATERTHEN = 1503
    LESSTHEN    = 1504
    TOLERANCE   = 1505
    AND         = 1506
    OR          = 1507

    # Keyword: DELAY Subkeywords (1600 -> 1699)
    UNTIL       = 1601

    # Keyword: RECORD Subkeywords (1700 -> 1799)
    EVERY       = 1700
    FOR         = 1701
    AS          = 1702

    # Splitting Tokens (2000 -> 2999)
    COLON       = 2001

    # Grouping Tokens (3000 -> 3999)
    OPENPAREN   = 3000 # (
    CLOSEPAREN  = 3001 # )
    ENDBLOCK    = 3002

    # Operator Tokens (4000 -> 4999)
    BINARYOP    = 4000
    EQUALS      = 4001

    # Builtin Hardware Functioncalls (5000 -> 5999)
    VOLTAGE     = 5000
    CURRENT     = 5001

    # Builtin Unit Functioncalls (6000 -> 6999)
    UNIT_PERCENT= 6000
    UNIT_VOLTS  = 6001
    UNIT_MILAMPS= 6002
    UNIT_MILSECS= 6003

    # File and Line Breaks (9000 -> 9999)
    EOL         = 9000
    EOF         = 9001


Keywords = {
    "WITH":         TokenType.WITH,
    "ON":           TokenType.ON,
    "CONSTANT":     TokenType.CONSTANT,
    "MATHFUNC":     TokenType.MATHFUNC,
    "SET":          TokenType.SET,
    "LABEL":        TokenType.LABEL,
    "ALWAYS":       TokenType.ALWAYS,
    "ASSERT":       TokenType.ASSERT,
    "CONTEXT":      TokenType.CONTEXT,      # TODO: Re-evaluate, is keyword or object in global scope?
    "HARDWARE":     TokenType.HARDWARE,     # TODO: Re-evaluate, is keyword or object in global scope?
    "NAME":         TokenType.NAME,         # TODO: Re-evaluate, is keyword or object in global scope?
    "AUTHOR":       TokenType.AUTHOR,       # TODO: Re-evaluate, is keyword or object in global scope?
    "GPO":          TokenType.GPO,
    "GPI":          TokenType.GPI,
    "NAMED":        TokenType.NAMED,
    "USING":        TokenType.USING,
    "FAILURE":      TokenType.FAILURE,
    "COMPLETE":     TokenType.COMPLETE,
    "LOW":          TokenType.LOW,          # TODO: Re-evaluate, is keyword or object in global scope?
    "HIGH":         TokenType.HIGH,         # TODO: Re-evaluate, is keyword or object in global scope?
    "WEAK":         TokenType.WEAK,
    "EQUAL":        TokenType.EQUAL,
    "MATCHES":      TokenType.MATCHES,
    "GREATERTHEN":  TokenType.GREATERTHEN,
    "LESSTHEN":     TokenType.LESSTHEN,
    "TOLERANCE":    TokenType.TOLERANCE,
    "AND":          TokenType.AND,
    "OR":           TokenType.OR,
    "END":          TokenType.ENDBLOCK,
    "VOLTAGE":      TokenType.VOLTAGE,      # TODO: Re-evaluate, is keyword or object in global scope?
    "CURRENT":      TokenType.CURRENT,      # TODO: Re-evaluate, is keyword or object in global scope?
    "UNIT_PERCENT": TokenType.UNIT_PERCENT, # TODO: Re-evaluate, is keyword or object in global scope?
    "UNIT_VOLTS":   TokenType.UNIT_VOLTS,   # TODO: Re-evaluate, is keyword or object in global scope?
    "UNIT_MILAMPS": TokenType.UNIT_MILAMPS, # TODO: Re-evaluate, is keyword or object in global scope?
    "UNIT_MILSECS": TokenType.UNIT_MILSECS, # TODO: Re-evaluate, is keyword or object in global scope?
    "RECORD":       TokenType.RECORD,
    "DELAY":        TokenType.DELAY,
    "UNTIL":        TokenType.UNTIL,
    "EVERY":        TokenType.EVERY,
    "FOR":          TokenType.FOR,
    "AS":           TokenType.AS,
    "ms":           TokenType.UNIT_MILSECS,
    "mA":           TokenType.UNIT_MILAMPS,
    "V":            TokenType.UNIT_VOLTS,

    "\n":           TokenType.EOL,
    "\n\r":         TokenType.EOL,
    "\r\n":         TokenType.EOL,
    "\r":           TokenType.EOL,
}


@dataclass
class Token:
    type: TokenType
    value: Optional[str]

    def __str__(self) -> str:
        return f"Token(\n\ttype: {self.type},\n\tvalue: \"{self.value}\"\n)"
    

def strTokens(tokens: List[Token]) -> str:
    result = "[\n"
    for token in tokens:
        result += f"\tToken(\n\t\ttype = {token.type},\n\t\tvalue = \"{token.value}\"\n\t),\n"
    result += "]"
    return result


class Lexer():
    def __token(self, value: str, type: TokenType) -> Token:
        return Token(type=type, value=value)


    def __is_alph(self, source: str) -> bool:
        return bool(re.match('^[a-zA-Z_]+$', source))


    def __is_num(self, source: str) -> bool:
        return bool(re.match('^[0-9.]+$', source))
    

    def __is_skippable(self, source: str) -> bool:
        # return bool(re.match('\s+', source))
        return source == ' ' or source == "\r" or source == "\t"


    def __is_eol(self, source: str) -> bool:
        return source == "\n"


    def Tokenize(self, source: str) -> List[Token]:
        tokens: List[Token] = []
        sourceChars = list(source)
        
        while(sourceChars):
            if sourceChars[0] == "(":
                tokens.append(self.__token(sourceChars.pop(0), TokenType.OPENPAREN))
            elif sourceChars[0] == ")":
                tokens.append(self.__token(sourceChars.pop(0), TokenType.CLOSEPAREN))            
            elif sourceChars[0] == "+" or sourceChars[0] == "-" or sourceChars[0] == "*" or sourceChars[0] == "/":
                tokens.append(self.__token(sourceChars.pop(0), TokenType.BINARYOP))
            elif sourceChars[0] == "%":
                if tokens[-2].type == TokenType.TOLERANCE: # % has different meaning depending on if it is preceded directly by a TOLERANCE keyword
                    tokens.append(self.__token(sourceChars.pop(0), TokenType.UNIT_PERCENT))
                else:
                    tokens.append(self.__token(sourceChars.pop(0), TokenType.BINARYOP))
            elif sourceChars[0] == "=":
                tokens.append(self.__token(sourceChars.pop(0), TokenType.EQUALS))
            elif sourceChars[0] == "@":
                tokens.append(self.__token(sourceChars.pop(0), TokenType.AT))  
            elif sourceChars[0] == ":":
                tokens.append(self.__token(sourceChars.pop(0), TokenType.COLON))
            elif self.__is_eol(sourceChars[0]):
                sourceChars.pop(0)
                tokens.append(self.__token("\\n", TokenType.EOL))
            elif sourceChars[0] == "#": # Comment drops rest of line
                while sourceChars and not self.__is_eol(sourceChars[0]):
                    sourceChars.pop(0)
            # Multi-character tokens
            else:
                # Numbers
                if self.__is_num(sourceChars[0]):
                    number = ""
                    while sourceChars and self.__is_num(sourceChars[0]):
                        number += sourceChars.pop(0)
                    tokens.append(self.__token(number, TokenType.NUMBER))
                # Identifiers
                elif self.__is_alph(sourceChars[0]):
                    identifier = ""
                    while sourceChars and self.__is_alph(sourceChars[0]):
                        identifier += sourceChars.pop(0)
                    if identifier in Keywords:
                        tokens.append(self.__token(identifier, Keywords[identifier]))                        
                    else:
                        tokens.append(self.__token(identifier, TokenType.IDENTIFIER))
                # Skippable whitespace
                elif self.__is_skippable(sourceChars[0]):
                    sourceChars.pop(0)
                else:
                    print(f"Unrecognized character found in source ({ord(sourceChars[0])}), starting with: {sourceChars[:min(10, len(sourceChars))]}")
                    return tokens

        tokens.append(self.__token("EOF", TokenType.EOF))
        return tokens


if __name__ == "__main__":
    with open("test.hil", "r") as f:
        data = f.read()
    lexer = Lexer()
    tokens = lexer.Tokenize(data)
    # print(json.dumps(tokens, indent=4, sort_keys=False))
    print(strTokens(tokens))
    with open("test.lex", "w") as f:
        f.write(strTokens(tokens))