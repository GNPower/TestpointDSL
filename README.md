To run the code on Niagara, take the following steps:

1. Clone this repository on Niagara
2. CD to the Code/Code directory in the repository (you should see a niagara.sh file)
3. chmod +x niagara.sh
4. Either:
    1. ./build.sh \&\& ./run.sh
    2. sbatch niagara.sh

The code will produce the following results:

1. The accuracy results data in Code/Code/plotGen/ accuracy\_results.txt
2. The accuracy results graph in Code/Code/plotGen/ accuracy\_results.png
3. The timing results data in Code/Code/plotGen/ timing\_results.txt
4. The timing results graph in Code/Code/plotGen/ timing\_results.png
5. The accuracy of the custom UCR Pulse Dataset in either the console or the niagara output job file
6. The AST resulting from source code in Code/Code/ transpilerDSL/test.hil in Code/Code/transpilerDSL/test.ast
7. The AST resulting from source code in Code/Code/ transpilerDSL/test2.hil in Code/Code/transpilerDSL/test2.ast
8. The execution results from source code in Code/Code/ transpilerDSL/test2.hil in Code/Code/transpilerDSL/test2.res