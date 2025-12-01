```mermaid
flowchart TD
    N0[Program]
    N1[compUnit]
    N0 --> N1
    N2[compUnit_list]
    N1 --> N2
    N3[compUnit_list]
    N2 --> N3
    N4[compUnit_item]
    N3 --> N4
    N5[decl]
    N4 --> N5
    N6[constDecl]
    N5 --> N6
    N7(("const"))
    N6 --> N7
    N8[bType]
    N6 --> N8
    N9(("int"))
    N8 --> N9
    N10[constDef]
    N6 --> N10
    N11(("GLOBAL_CONST"))
    N10 --> N11
    N12(("="))
    N10 --> N12
    N13[constInitVal]
    N10 --> N13
    N14[constExp]
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
    N20(("100"))
    N19 --> N20
    N21[constDef_list]
    N6 --> N21
    N22((";"))
    N6 --> N22
    N23[compUnit_item]
    N2 --> N23
    N24[funcDef]
    N23 --> N24
    N25[bType]
    N24 --> N25
    N26(("int"))
    N25 --> N26
    N27(("main"))
    N24 --> N27
    N28(("("))
    N24 --> N28
    N29[funcFParams_opt]
    N24 --> N29
    N30((")"))
    N24 --> N30
    N31[block]
    N24 --> N31
    N32(("{"))
    N31 --> N32
    N33[blockItem_list]
    N31 --> N33
    N34[blockItem_list]
    N33 --> N34
    N35[blockItem_list]
    N34 --> N35
    N36[blockItem_list]
    N35 --> N36
    N37[blockItem_list]
    N36 --> N37
    N38[blockItem_list]
    N37 --> N38
    N39[blockItem_list]
    N38 --> N39
    N40[blockItem_list]
    N39 --> N40
    N41[blockItem]
    N39 --> N41
    N42[decl]
    N41 --> N42
    N43[constDecl]
    N42 --> N43
    N44(("const"))
    N43 --> N44
    N45[bType]
    N43 --> N45
    N46(("int"))
    N45 --> N46
    N47[constDef]
    N43 --> N47
    N48(("LOCAL_CONST"))
    N47 --> N48
    N49(("="))
    N47 --> N49
    N50[constInitVal]
    N47 --> N50
    N51[constExp]
    N50 --> N51
    N52[addExp]
    N51 --> N52
    N53[mulExp]
    N52 --> N53
    N54[unaryExp]
    N53 --> N54
    N55[primaryExp]
    N54 --> N55
    N56[number]
    N55 --> N56
    N57(("50"))
    N56 --> N57
    N58[constDef_list]
    N43 --> N58
    N59((";"))
    N43 --> N59
    N60[blockItem]
    N38 --> N60
    N61[decl]
    N60 --> N61
    N62[varDecl]
    N61 --> N62
    N63[bType]
    N62 --> N63
    N64(("int"))
    N63 --> N64
    N65[varDef]
    N62 --> N65
    N66(("a"))
    N65 --> N66
    N67(("="))
    N65 --> N67
    N68[initVal]
    N65 --> N68
    N69[exp]
    N68 --> N69
    N70[addExp]
    N69 --> N70
    N71[mulExp]
    N70 --> N71
    N72[unaryExp]
    N71 --> N72
    N73[primaryExp]
    N72 --> N73
    N74[lVal]
    N73 --> N74
    N75(("GLOBAL_CONST"))
    N74 --> N75
    N76[varDef_list]
    N62 --> N76
    N77((";"))
    N62 --> N77
    N78[blockItem]
    N37 --> N78
    N79[decl]
    N78 --> N79
    N80[varDecl]
    N79 --> N80
    N81[bType]
    N80 --> N81
    N82(("int"))
    N81 --> N82
    N83[varDef]
    N80 --> N83
    N84(("b"))
    N83 --> N84
    N85(("="))
    N83 --> N85
    N86[initVal]
    N83 --> N86
    N87[exp]
    N86 --> N87
    N88[addExp]
    N87 --> N88
    N89[mulExp]
    N88 --> N89
    N90[unaryExp]
    N89 --> N90
    N91[primaryExp]
    N90 --> N91
    N92[lVal]
    N91 --> N92
    N93(("LOCAL_CONST"))
    N92 --> N93
    N94[varDef_list]
    N80 --> N94
    N95((";"))
    N80 --> N95
    N96[blockItem]
    N36 --> N96
    N97[decl]
    N96 --> N97
    N98[constDecl]
    N97 --> N98
    N99(("const"))
    N98 --> N99
    N100[bType]
    N98 --> N100
    N101(("float"))
    N100 --> N101
    N102[constDef]
    N98 --> N102
    N103(("PI"))
    N102 --> N103
    N104(("="))
    N102 --> N104
    N105[constInitVal]
    N102 --> N105
    N106[constExp]
    N105 --> N106
    N107[addExp]
    N106 --> N107
    N108[mulExp]
    N107 --> N108
    N109[unaryExp]
    N108 --> N109
    N110[primaryExp]
    N109 --> N110
    N111[number]
    N110 --> N111
    N112(("3.14"))
    N111 --> N112
    N113[constDef_list]
    N98 --> N113
    N114((";"))
    N98 --> N114
    N115[blockItem]
    N35 --> N115
    N116[decl]
    N115 --> N116
    N117[varDecl]
    N116 --> N117
    N118[bType]
    N117 --> N118
    N119(("float"))
    N118 --> N119
    N120[varDef]
    N117 --> N120
    N121(("r"))
    N120 --> N121
    N122(("="))
    N120 --> N122
    N123[initVal]
    N120 --> N123
    N124[exp]
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
    N130(("10.0"))
    N129 --> N130
    N131[varDef_list]
    N117 --> N131
    N132((";"))
    N117 --> N132
    N133[blockItem]
    N34 --> N133
    N134[decl]
    N133 --> N134
    N135[varDecl]
    N134 --> N135
    N136[bType]
    N135 --> N136
    N137(("float"))
    N136 --> N137
    N138[varDef]
    N135 --> N138
    N139(("area"))
    N138 --> N139
    N140(("="))
    N138 --> N140
    N141[initVal]
    N138 --> N141
    N142[exp]
    N141 --> N142
    N143[addExp]
    N142 --> N143
    N144[mulExp]
    N143 --> N144
    N145[mulExp]
    N144 --> N145
    N146[mulExp]
    N145 --> N146
    N147[unaryExp]
    N146 --> N147
    N148[primaryExp]
    N147 --> N148
    N149[lVal]
    N148 --> N149
    N150(("PI"))
    N149 --> N150
    N151(("*"))
    N145 --> N151
    N152[unaryExp]
    N145 --> N152
    N153[primaryExp]
    N152 --> N153
    N154[lVal]
    N153 --> N154
    N155(("r"))
    N154 --> N155
    N156(("*"))
    N144 --> N156
    N157[unaryExp]
    N144 --> N157
    N158[primaryExp]
    N157 --> N158
    N159[lVal]
    N158 --> N159
    N160(("r"))
    N159 --> N160
    N161[varDef_list]
    N135 --> N161
    N162((";"))
    N135 --> N162
    N163[blockItem]
    N33 --> N163
    N164[stmt]
    N163 --> N164
    N165(("return"))
    N164 --> N165
    N166[exp_opt]
    N164 --> N166
    N167[exp]
    N166 --> N167
    N168[addExp]
    N167 --> N168
    N169[mulExp]
    N168 --> N169
    N170[unaryExp]
    N169 --> N170
    N171[primaryExp]
    N170 --> N171
    N172[number]
    N171 --> N172
    N173(("0"))
    N172 --> N173
    N174((";"))
    N164 --> N174
    N175(("}"))
    N31 --> N175
    N176(("}"))
    N1 --> N176
```
