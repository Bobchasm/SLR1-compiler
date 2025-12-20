```mermaid
flowchart TD
    N0[Program]
    N1[compUnit]
    N0 --> N1
    N2[compUnit_list]
    N1 --> N2
    N3[compUnit_item]
    N2 --> N3
    N4[funcDef]
    N3 --> N4
    N5[bType]
    N4 --> N5
    N6(("float"))
    N5 --> N6
    N7(("main"))
    N4 --> N7
    N8(("("))
    N4 --> N8
    N9[funcFParams_opt]
    N4 --> N9
    N10((")"))
    N4 --> N10
    N11[block]
    N4 --> N11
    N12(("{"))
    N11 --> N12
    N13[blockItem_list]
    N11 --> N13
    N14[blockItem_list]
    N13 --> N14
    N15[blockItem_list]
    N14 --> N15
    N16[blockItem]
    N14 --> N16
    N17[decl]
    N16 --> N17
    N18[varDecl]
    N17 --> N18
    N19[bType]
    N18 --> N19
    N20(("float"))
    N19 --> N20
    N21[varDef]
    N18 --> N21
    N22(("b"))
    N21 --> N22
    N23(("="))
    N21 --> N23
    N24[initVal]
    N21 --> N24
    N25[exp]
    N24 --> N25
    N26[assignExp]
    N25 --> N26
    N27[lOrExp]
    N26 --> N27
    N28[lAndExp]
    N27 --> N28
    N29[eqExp]
    N28 --> N29
    N30[relExp]
    N29 --> N30
    N31[addExp]
    N30 --> N31
    N32[mulExp]
    N31 --> N32
    N33[unaryExp]
    N32 --> N33
    N34[primaryExp]
    N33 --> N34
    N35[number]
    N34 --> N35
    N36(("3"))
    N35 --> N36
    N37[varDef_list]
    N18 --> N37
    N38((";"))
    N18 --> N38
    N39[blockItem]
    N13 --> N39
    N40[stmt]
    N39 --> N40
    N41(("return"))
    N40 --> N41
    N42[exp_opt]
    N40 --> N42
    N43[exp]
    N42 --> N43
    N44[assignExp]
    N43 --> N44
    N45[lOrExp]
    N44 --> N45
    N46[lAndExp]
    N45 --> N46
    N47[eqExp]
    N46 --> N47
    N48[relExp]
    N47 --> N48
    N49[addExp]
    N48 --> N49
    N50[mulExp]
    N49 --> N50
    N51[unaryExp]
    N50 --> N51
    N52[primaryExp]
    N51 --> N52
    N53[lVal]
    N52 --> N53
    N54(("b"))
    N53 --> N54
    N55((";"))
    N40 --> N55
    N56(("}"))
    N11 --> N56
    N57(("}"))
    N1 --> N57
```
