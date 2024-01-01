import argparse
from frontend.transpiler_parser import Parser
from runtime.interpreter import evaluate
from runtime.scope import Scope
from runtime.values import NumberValue, NullValue, BooleanValue


def parseArgs() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        prog="Testpoint DSL",
        description="Domain specific language for HIL testing"
    )
    parser.add_argument("-r", "--do-run", type=str, default=False, help="Whether or not you should run the interpreter")
    parser.add_argument("-f", "--file", type=str, default="", help="The source code to interpret. If no file is given a repl loop runs instead")
    return parser.parse_args()


def interpret(dorun: bool, fileName: str):
    parser = Parser()
    globalScope = Scope()
    print("\nInterpreter v0.1")
    with open(f"{fileName}.hil", "r") as f:
        source = f.read()
    program = parser.produceAST(source)        
    print(program)
    with open(f"{fileName}.ast", "w") as f:
        f.write(program.__str__())

    if dorun == "true":
        result = evaluate(program, globalScope)
        print(result)
        with open(f"{fileName}.res", "w") as f:
            f.write(result.__str__())


def repl():
    parser = Parser()
    globalScope = Scope()
    print("\nRepl v0.1")
    while(1):
        source = input("> ")
        if not source or "exit" in source:
            break
        program = parser.produceAST(source)        
        print(program)
        result = evaluate(program, globalScope)        
        print(result)


if __name__ == "__main__":
    args = parseArgs()
    if args.file:
        interpret(args.do_run, args.file)
    else:
        repl()