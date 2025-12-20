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
    N6(("void"))
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
    N16[blockItem_list]
    N15 --> N16
    N17[blockItem_list]
    N16 --> N17
    N18[blockItem]
    N16 --> N18
    N19[decl]
    N18 --> N19
    N20[varDecl]
    N19 --> N20
    N21[bType]
    N20 --> N21
    N22(("float"))
    N21 --> N22
    N23[varDef]
    N20 --> N23
    N24(("a"))
    N23 --> N24
    N25(("="))
    N23 --> N25
    N26[initVal]
    N23 --> N26
    N27[exp]
    N26 --> N27
    N28[assignExp]
    N27 --> N28
    N29[lOrExp]
    N28 --> N29
    N30[lAndExp]
    N29 --> N30
    N31[eqExp]
    N30 --> N31
    N32[relExp]
    N31 --> N32
    N33[addExp]
    N32 --> N33
    N34[mulExp]
    N33 --> N34
    N35[unaryExp]
    N34 --> N35
    N36[primaryExp]
    N35 --> N36
    N37[number]
    N36 --> N37
    N38(("0.1"))
    N37 --> N38
    N39[varDef_list]
    N20 --> N39
    N40((";"))
    N20 --> N40
    N41[blockItem]
    N15 --> N41
    N42[decl]
    N41 --> N42
    N43[varDecl]
    N42 --> N43
    N44[bType]
    N43 --> N44
    N45(("float"))
    N44 --> N45
    N46[varDef]
    N43 --> N46
    N47(("b"))
    N46 --> N47
    N48(("="))
    N46 --> N48
    N49[initVal]
    N46 --> N49
    N50[exp]
    N49 --> N50
    N51[assignExp]
    N50 --> N51
    N52[lOrExp]
    N51 --> N52
    N53[lAndExp]
    N52 --> N53
    N54[eqExp]
    N53 --> N54
    N55[relExp]
    N54 --> N55
    N56[addExp]
    N55 --> N56
    N57[mulExp]
    N56 --> N57
    N58[unaryExp]
    N57 --> N58
    N59[primaryExp]
    N58 --> N59
    N60[number]
    N59 --> N60
    N61(("0.2"))
    N60 --> N61
    N62[varDef_list]
    N43 --> N62
    N63((";"))
    N43 --> N63
    N64[blockItem]
    N14 --> N64
    N65[decl]
    N64 --> N65
    N66[varDecl]
    N65 --> N66
    N67[bType]
    N66 --> N67
    N68(("float"))
    N67 --> N68
    N69[varDef]
    N66 --> N69
    N70(("c"))
    N69 --> N70
    N71(("="))
    N69 --> N71
    N72[initVal]
    N69 --> N72
    N73[exp]
    N72 --> N73
    N74[assignExp]
    N73 --> N74
    N75[lOrExp]
    N74 --> N75
    N76[lAndExp]
    N75 --> N76
    N77[eqExp]
    N76 --> N77
    N78[relExp]
    N77 --> N78
    N79[addExp]
    N78 --> N79
    N80[mulExp]
    N79 --> N80
    N81[unaryExp]
    N80 --> N81
    N82[primaryExp]
    N81 --> N82
    N83[number]
    N82 --> N83
    N84(("0.3"))
    N83 --> N84
    N85[varDef_list]
    N66 --> N85
    N86((";"))
    N66 --> N86
    N87[blockItem]
    N13 --> N87
    N88[stmt]
    N87 --> N88
    N89(("if"))
    N88 --> N89
    N90(("("))
    N88 --> N90
    N91[cond]
    N88 --> N91
    N92[lOrExp]
    N91 --> N92
    N93[lAndExp]
    N92 --> N93
    N94[eqExp]
    N93 --> N94
    N95[eqExp]
    N94 --> N95
    N96[relExp]
    N95 --> N96
    N97[addExp]
    N96 --> N97
    N98[addExp]
    N97 --> N98
    N99[mulExp]
    N98 --> N99
    N100[unaryExp]
    N99 --> N100
    N101[primaryExp]
    N100 --> N101
    N102[lVal]
    N101 --> N102
    N103(("a"))
    N102 --> N103
    N104(("+"))
    N97 --> N104
    N105[mulExp]
    N97 --> N105
    N106[unaryExp]
    N105 --> N106
    N107[primaryExp]
    N106 --> N107
    N108[lVal]
    N107 --> N108
    N109(("b"))
    N108 --> N109
    N110(("=="))
    N94 --> N110
    N111[relExp]
    N94 --> N111
    N112[addExp]
    N111 --> N112
    N113[mulExp]
    N112 --> N113
    N114[unaryExp]
    N113 --> N114
    N115[primaryExp]
    N114 --> N115
    N116[lVal]
    N115 --> N116
    N117(("c"))
    N116 --> N117
    N118((")"))
    N88 --> N118
    N119[stmt]
    N88 --> N119
    N120[block]
    N119 --> N120
    N121(("{"))
    N120 --> N121
    N122[blockItem_list]
    N120 --> N122
    N123(("}"))
    N120 --> N123
    N124[else_opt]
    N88 --> N124
    N125(("}"))
    N11 --> N125
    N126(("}"))
    N1 --> N126
```
