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
    N6(("int"))
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
    N38(("3.14"))
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
    N61(("2.71"))
    N60 --> N61
    N62[varDef_list]
    N43 --> N62
    N63((";"))
    N43 --> N63
    N64[blockItem]
    N14 --> N64
    N65[stmt]
    N64 --> N65
    N66(("if"))
    N65 --> N66
    N67(("("))
    N65 --> N67
    N68[cond]
    N65 --> N68
    N69[lOrExp]
    N68 --> N69
    N70[lAndExp]
    N69 --> N70
    N71[eqExp]
    N70 --> N71
    N72[relExp]
    N71 --> N72
    N73[relExp]
    N72 --> N73
    N74[addExp]
    N73 --> N74
    N75[mulExp]
    N74 --> N75
    N76[unaryExp]
    N75 --> N76
    N77[primaryExp]
    N76 --> N77
    N78[lVal]
    N77 --> N78
    N79(("a"))
    N78 --> N79
    N80((">"))
    N72 --> N80
    N81[addExp]
    N72 --> N81
    N82[mulExp]
    N81 --> N82
    N83[unaryExp]
    N82 --> N83
    N84[primaryExp]
    N83 --> N84
    N85[lVal]
    N84 --> N85
    N86(("b"))
    N85 --> N86
    N87((")"))
    N65 --> N87
    N88[stmt]
    N65 --> N88
    N89[block]
    N88 --> N89
    N90(("{"))
    N89 --> N90
    N91[blockItem_list]
    N89 --> N91
    N92[blockItem_list]
    N91 --> N92
    N93[blockItem]
    N91 --> N93
    N94[stmt]
    N93 --> N94
    N95[exp_opt]
    N94 --> N95
    N96[exp]
    N95 --> N96
    N97[assignExp]
    N96 --> N97
    N98[lVal]
    N97 --> N98
    N99(("a"))
    N98 --> N99
    N100(("="))
    N97 --> N100
    N101[assignExp]
    N97 --> N101
    N102[lOrExp]
    N101 --> N102
    N103[lAndExp]
    N102 --> N103
    N104[eqExp]
    N103 --> N104
    N105[relExp]
    N104 --> N105
    N106[addExp]
    N105 --> N106
    N107[mulExp]
    N106 --> N107
    N108[unaryExp]
    N107 --> N108
    N109[primaryExp]
    N108 --> N109
    N110[number]
    N109 --> N110
    N111(("1.2"))
    N110 --> N111
    N112((";"))
    N94 --> N112
    N113(("}"))
    N89 --> N113
    N114[else_opt]
    N65 --> N114
    N115[blockItem]
    N13 --> N115
    N116[stmt]
    N115 --> N116
    N117(("return"))
    N116 --> N117
    N118[exp_opt]
    N116 --> N118
    N119[exp]
    N118 --> N119
    N120[assignExp]
    N119 --> N120
    N121[lOrExp]
    N120 --> N121
    N122[lAndExp]
    N121 --> N122
    N123[eqExp]
    N122 --> N123
    N124[relExp]
    N123 --> N124
    N125[addExp]
    N124 --> N125
    N126[mulExp]
    N125 --> N126
    N127[unaryExp]
    N126 --> N127
    N128[primaryExp]
    N127 --> N128
    N129[number]
    N128 --> N129
    N130(("0"))
    N129 --> N130
    N131((";"))
    N116 --> N131
    N132(("}"))
    N11 --> N132
    N133(("}"))
    N1 --> N133
```
