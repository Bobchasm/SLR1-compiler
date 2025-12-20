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
    N18[blockItem_list]
    N17 --> N18
    N19[blockItem]
    N17 --> N19
    N20[decl]
    N19 --> N20
    N21[varDecl]
    N20 --> N21
    N22[bType]
    N21 --> N22
    N23(("int"))
    N22 --> N23
    N24[varDef]
    N21 --> N24
    N25(("a"))
    N24 --> N25
    N26(("="))
    N24 --> N26
    N27[initVal]
    N24 --> N27
    N28[exp]
    N27 --> N28
    N29[assignExp]
    N28 --> N29
    N30[lOrExp]
    N29 --> N30
    N31[lAndExp]
    N30 --> N31
    N32[eqExp]
    N31 --> N32
    N33[relExp]
    N32 --> N33
    N34[addExp]
    N33 --> N34
    N35[mulExp]
    N34 --> N35
    N36[unaryExp]
    N35 --> N36
    N37[primaryExp]
    N36 --> N37
    N38[number]
    N37 --> N38
    N39(("10"))
    N38 --> N39
    N40[varDef_list]
    N21 --> N40
    N41((";"))
    N21 --> N41
    N42[blockItem]
    N16 --> N42
    N43[decl]
    N42 --> N43
    N44[varDecl]
    N43 --> N44
    N45[bType]
    N44 --> N45
    N46(("int"))
    N45 --> N46
    N47[varDef]
    N44 --> N47
    N48(("b"))
    N47 --> N48
    N49(("="))
    N47 --> N49
    N50[initVal]
    N47 --> N50
    N51[exp]
    N50 --> N51
    N52[assignExp]
    N51 --> N52
    N53[lOrExp]
    N52 --> N53
    N54[lAndExp]
    N53 --> N54
    N55[eqExp]
    N54 --> N55
    N56[relExp]
    N55 --> N56
    N57[addExp]
    N56 --> N57
    N58[mulExp]
    N57 --> N58
    N59[unaryExp]
    N58 --> N59
    N60[primaryExp]
    N59 --> N60
    N61[number]
    N60 --> N61
    N62(("20"))
    N61 --> N62
    N63[varDef_list]
    N44 --> N63
    N64((";"))
    N44 --> N64
    N65[blockItem]
    N15 --> N65
    N66[stmt]
    N65 --> N66
    N67(("if"))
    N66 --> N67
    N68(("("))
    N66 --> N68
    N69[cond]
    N66 --> N69
    N70[lOrExp]
    N69 --> N70
    N71[lAndExp]
    N70 --> N71
    N72[eqExp]
    N71 --> N72
    N73[relExp]
    N72 --> N73
    N74[relExp]
    N73 --> N74
    N75[addExp]
    N74 --> N75
    N76[mulExp]
    N75 --> N76
    N77[unaryExp]
    N76 --> N77
    N78[primaryExp]
    N77 --> N78
    N79[lVal]
    N78 --> N79
    N80(("a"))
    N79 --> N80
    N81(("<"))
    N73 --> N81
    N82[addExp]
    N73 --> N82
    N83[mulExp]
    N82 --> N83
    N84[unaryExp]
    N83 --> N84
    N85[primaryExp]
    N84 --> N85
    N86[lVal]
    N85 --> N86
    N87(("b"))
    N86 --> N87
    N88((")"))
    N66 --> N88
    N89[stmt]
    N66 --> N89
    N90[block]
    N89 --> N90
    N91(("{"))
    N90 --> N91
    N92[blockItem_list]
    N90 --> N92
    N93[blockItem_list]
    N92 --> N93
    N94[blockItem]
    N92 --> N94
    N95[stmt]
    N94 --> N95
    N96[exp_opt]
    N95 --> N96
    N97[exp]
    N96 --> N97
    N98[assignExp]
    N97 --> N98
    N99[lVal]
    N98 --> N99
    N100(("a"))
    N99 --> N100
    N101(("="))
    N98 --> N101
    N102[assignExp]
    N98 --> N102
    N103[lOrExp]
    N102 --> N103
    N104[lAndExp]
    N103 --> N104
    N105[eqExp]
    N104 --> N105
    N106[relExp]
    N105 --> N106
    N107[addExp]
    N106 --> N107
    N108[mulExp]
    N107 --> N108
    N109[unaryExp]
    N108 --> N109
    N110[primaryExp]
    N109 --> N110
    N111[lVal]
    N110 --> N111
    N112(("b"))
    N111 --> N112
    N113((";"))
    N95 --> N113
    N114(("}"))
    N90 --> N114
    N115[else_opt]
    N66 --> N115
    N116(("else"))
    N115 --> N116
    N117[stmt]
    N115 --> N117
    N118[block]
    N117 --> N118
    N119(("{"))
    N118 --> N119
    N120[blockItem_list]
    N118 --> N120
    N121[blockItem_list]
    N120 --> N121
    N122[blockItem]
    N120 --> N122
    N123[stmt]
    N122 --> N123
    N124[exp_opt]
    N123 --> N124
    N125[exp]
    N124 --> N125
    N126[assignExp]
    N125 --> N126
    N127[lVal]
    N126 --> N127
    N128(("b"))
    N127 --> N128
    N129(("="))
    N126 --> N129
    N130[assignExp]
    N126 --> N130
    N131[lOrExp]
    N130 --> N131
    N132[lAndExp]
    N131 --> N132
    N133[eqExp]
    N132 --> N133
    N134[relExp]
    N133 --> N134
    N135[addExp]
    N134 --> N135
    N136[mulExp]
    N135 --> N136
    N137[unaryExp]
    N136 --> N137
    N138[primaryExp]
    N137 --> N138
    N139[lVal]
    N138 --> N139
    N140(("a"))
    N139 --> N140
    N141((";"))
    N123 --> N141
    N142(("}"))
    N118 --> N142
    N143[blockItem]
    N14 --> N143
    N144[stmt]
    N143 --> N144
    N145(("if"))
    N144 --> N145
    N146(("("))
    N144 --> N146
    N147[cond]
    N144 --> N147
    N148[lOrExp]
    N147 --> N148
    N149[lAndExp]
    N148 --> N149
    N150[eqExp]
    N149 --> N150
    N151[eqExp]
    N150 --> N151
    N152[relExp]
    N151 --> N152
    N153[addExp]
    N152 --> N153
    N154[mulExp]
    N153 --> N154
    N155[unaryExp]
    N154 --> N155
    N156[primaryExp]
    N155 --> N156
    N157[lVal]
    N156 --> N157
    N158(("a"))
    N157 --> N158
    N159(("=="))
    N150 --> N159
    N160[relExp]
    N150 --> N160
    N161[addExp]
    N160 --> N161
    N162[mulExp]
    N161 --> N162
    N163[unaryExp]
    N162 --> N163
    N164[primaryExp]
    N163 --> N164
    N165[lVal]
    N164 --> N165
    N166(("b"))
    N165 --> N166
    N167((")"))
    N144 --> N167
    N168[stmt]
    N144 --> N168
    N169[block]
    N168 --> N169
    N170(("{"))
    N169 --> N170
    N171[blockItem_list]
    N169 --> N171
    N172[blockItem_list]
    N171 --> N172
    N173[blockItem]
    N171 --> N173
    N174[stmt]
    N173 --> N174
    N175(("return"))
    N174 --> N175
    N176[exp_opt]
    N174 --> N176
    N177[exp]
    N176 --> N177
    N178[assignExp]
    N177 --> N178
    N179[lOrExp]
    N178 --> N179
    N180[lAndExp]
    N179 --> N180
    N181[eqExp]
    N180 --> N181
    N182[relExp]
    N181 --> N182
    N183[addExp]
    N182 --> N183
    N184[mulExp]
    N183 --> N184
    N185[unaryExp]
    N184 --> N185
    N186[primaryExp]
    N185 --> N186
    N187[number]
    N186 --> N187
    N188(("1"))
    N187 --> N188
    N189((";"))
    N174 --> N189
    N190(("}"))
    N169 --> N190
    N191[else_opt]
    N144 --> N191
    N192[blockItem]
    N13 --> N192
    N193[stmt]
    N192 --> N193
    N194(("return"))
    N193 --> N194
    N195[exp_opt]
    N193 --> N195
    N196[exp]
    N195 --> N196
    N197[assignExp]
    N196 --> N197
    N198[lOrExp]
    N197 --> N198
    N199[lAndExp]
    N198 --> N199
    N200[eqExp]
    N199 --> N200
    N201[relExp]
    N200 --> N201
    N202[addExp]
    N201 --> N202
    N203[mulExp]
    N202 --> N203
    N204[unaryExp]
    N203 --> N204
    N205[primaryExp]
    N204 --> N205
    N206[number]
    N205 --> N206
    N207(("0"))
    N206 --> N207
    N208((";"))
    N193 --> N208
    N209(("}"))
    N11 --> N209
    N210(("}"))
    N1 --> N210
```
