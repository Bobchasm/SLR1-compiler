```mermaid
flowchart TD
    N0[Program]
    N1[compUnit]
    N0 --> N1
    N2[compUnit_list]
    N1 --> N2
    N3[compUnit_list]
    N2 --> N3
    N4[compUnit_list]
    N3 --> N4
    N5[compUnit_item]
    N3 --> N5
    N6[decl]
    N5 --> N6
    N7[varDecl]
    N6 --> N7
    N8[bType]
    N7 --> N8
    N9((int))
    N8 --> N9
    N10[varDef]
    N7 --> N10
    N11((a))
    N10 --> N11
    N12((T12))
    N10 --> N12
    N13[initVal]
    N10 --> N13
    N14[exp]
    N13 --> N14
    N15[addExp]
    N14 --> N15
    N16[mulExp]
    N15 --> N16
    N17[unaryExp]
    N16 --> N17
    N18[primaryExp]
    N17 --> N18
    N19[number]
    N18 --> N19
    N20((10))
    N19 --> N20
    N21[varDef_list]
    N7 --> N21
    N22((T22))
    N7 --> N22
    N23[compUnit_item]
    N2 --> N23
    N24[funcDef]
    N23 --> N24
    N25[bType]
    N24 --> N25
    N26((int))
    N25 --> N26
    N27((main))
    N24 --> N27
    N28((T28))
    N24 --> N28
    N29[funcFParams_opt]
    N24 --> N29
    N30((T30))
    N24 --> N30
    N31[block]
    N24 --> N31
    N32((T32))
    N31 --> N32
    N33[blockItem_list]
    N31 --> N33
    N34[blockItem_list]
    N33 --> N34
    N35[blockItem_list]
    N34 --> N35
    N36[blockItem]
    N34 --> N36
    N37[stmt]
    N36 --> N37
    N38[lVal]
    N37 --> N38
    N39((a))
    N38 --> N39
    N40((T40))
    N37 --> N40
    N41[exp]
    N37 --> N41
    N42[addExp]
    N41 --> N42
    N43[mulExp]
    N42 --> N43
    N44[unaryExp]
    N43 --> N44
    N45[primaryExp]
    N44 --> N45
    N46[number]
    N45 --> N46
    N47((10))
    N46 --> N47
    N48((T48))
    N37 --> N48
    N49[blockItem]
    N33 --> N49
    N50[stmt]
    N49 --> N50
    N51((return))
    N50 --> N51
    N52[exp_opt]
    N50 --> N52
    N53[exp]
    N52 --> N53
    N54[addExp]
    N53 --> N54
    N55[mulExp]
    N54 --> N55
    N56[unaryExp]
    N55 --> N56
    N57[primaryExp]
    N56 --> N57
    N58[number]
    N57 --> N58
    N59((0))
    N58 --> N59
    N60((T60))
    N50 --> N60
    N61((T61))
    N31 --> N61
    N62((T62))
    N1 --> N62
```
