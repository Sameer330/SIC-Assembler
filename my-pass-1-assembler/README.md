# Implementation of PASS-1 Algorithm

## Files(5 no's)
  1. my-pass-1.c
  2. input.txt
  3. optab.txt
  4. intermediate.txt
  5. symtab.txt

## Rules to write SIC Program in 'input.txt'
  1. All Instructions must be in Upper Case Letters.
  2. Each line contains 3 parts - label, opcode & operand.
  3. label and/or operand may or may not be present in an instruction.
  4. In all instructions, at least an opcode is expected.
  5. All opcodes must be present in 'optab.txt'
  6. Any operand must not contain a whitespace in it.
     Example: 
     | "LDA A, B" --- is not accepted  | "LDA A,B" --- is accepted |
  7. All instructions must be written in 'input.txt' (Proper indentations are encouraged)
  8. Please do not add comment lines in the program as there is not mechanism to handle it.

## Program Execution
  1. Add program to 'input.txt'
  2. Ensure 'optab.txt' contains all permitted opcodes. (A sample optab is provided in this repo.)
  3. Execute 'my-pass-1.c' file
  4. 'my-pass-1.o' and 'my-pass-1.exe' are created.
  5. Check 'intermediate.txt' for Instruction Addresses.
  6. Check 'symtab.txt' for (Label, Address)

### Created by Sameer Ankalagi
### GitHub.com/Sameer330
