from .transpiler_lexer import TokenType


class objectGPO():
    
    def __init__(self) -> None:
        pass
 

class objectGPI():
    pass


SupportedHardwareObjects = {
    TokenType.GPO: objectGPO,
    TokenType.GPI: objectGPI,
}