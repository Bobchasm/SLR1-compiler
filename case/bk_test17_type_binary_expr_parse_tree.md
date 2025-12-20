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
    N5[compUnit_list]
    N4 --> N5
    N6[compUnit_list]
    N5 --> N6
    N7[compUnit_item]
    N6 --> N7
    N8[funcDef]
    N7 --> N8
    N9[bType]
    N8 --> N9
    N10(("float"))
    N9 --> N10
    N11(("test_float_add"))
    N8 --> N11
    N12(("("))
    N8 --> N12
    N13[funcFParams_opt]
    N8 --> N13
    N14((")"))
    N8 --> N14
    N15[block]
    N8 --> N15
    N16(("{"))
    N15 --> N16
    N17[blockItem_list]
    N15 --> N17
    N18[blockItem_list]
    N17 --> N18
    N19[blockItem_list]
    N18 --> N19
    N20[blockItem_list]
    N19 --> N20
    N21[blockItem]
    N19 --> N21
    N22[decl]
    N21 --> N22
    N23[varDecl]
    N22 --> N23
    N24[bType]
    N23 --> N24
    N25(("float"))
    N24 --> N25
    N26[varDef]
    N23 --> N26
    N27(("a"))
    N26 --> N27
    N28(("="))
    N26 --> N28
    N29[initVal]
    N26 --> N29
    N30[exp]
    N29 --> N30
    N31[assignExp]
    N30 --> N31
    N32[lOrExp]
    N31 --> N32
    N33[lAndExp]
    N32 --> N33
    N34[eqExp]
    N33 --> N34
    N35[relExp]
    N34 --> N35
    N36[addExp]
    N35 --> N36
    N37[mulExp]
    N36 --> N37
    N38[unaryExp]
    N37 --> N38
    N39[primaryExp]
    N38 --> N39
    N40[number]
    N39 --> N40
    N41(("1.2"))
    N40 --> N41
    N42[varDef_list]
    N23 --> N42
    N43((";"))
    N23 --> N43
    N44[blockItem]
    N18 --> N44
    N45[decl]
    N44 --> N45
    N46[varDecl]
    N45 --> N46
    N47[bType]
    N46 --> N47
    N48(("float"))
    N47 --> N48
    N49[varDef]
    N46 --> N49
    N50(("b"))
    N49 --> N50
    N51(("="))
    N49 --> N51
    N52[initVal]
    N49 --> N52
    N53[exp]
    N52 --> N53
    N54[assignExp]
    N53 --> N54
    N55[lOrExp]
    N54 --> N55
    N56[lAndExp]
    N55 --> N56
    N57[eqExp]
    N56 --> N57
    N58[relExp]
    N57 --> N58
    N59[addExp]
    N58 --> N59
    N60[mulExp]
    N59 --> N60
    N61[unaryExp]
    N60 --> N61
    N62[primaryExp]
    N61 --> N62
    N63[number]
    N62 --> N63
    N64(("2.1"))
    N63 --> N64
    N65[varDef_list]
    N46 --> N65
    N66((";"))
    N46 --> N66
    N67[blockItem]
    N17 --> N67
    N68[stmt]
    N67 --> N68
    N69(("return"))
    N68 --> N69
    N70[exp_opt]
    N68 --> N70
    N71[exp]
    N70 --> N71
    N72[assignExp]
    N71 --> N72
    N73[lOrExp]
    N72 --> N73
    N74[lAndExp]
    N73 --> N74
    N75[eqExp]
    N74 --> N75
    N76[relExp]
    N75 --> N76
    N77[addExp]
    N76 --> N77
    N78[addExp]
    N77 --> N78
    N79[mulExp]
    N78 --> N79
    N80[unaryExp]
    N79 --> N80
    N81[primaryExp]
    N80 --> N81
    N82[lVal]
    N81 --> N82
    N83(("a"))
    N82 --> N83
    N84(("+"))
    N77 --> N84
    N85[mulExp]
    N77 --> N85
    N86[unaryExp]
    N85 --> N86
    N87[primaryExp]
    N86 --> N87
    N88[lVal]
    N87 --> N88
    N89(("b"))
    N88 --> N89
    N90((";"))
    N68 --> N90
    N91(("}"))
    N15 --> N91
    N92[compUnit_item]
    N5 --> N92
    N93[funcDef]
    N92 --> N93
    N94[bType]
    N93 --> N94
    N95(("int"))
    N94 --> N95
    N96(("test_int_add"))
    N93 --> N96
    N97(("("))
    N93 --> N97
    N98[funcFParams_opt]
    N93 --> N98
    N99((")"))
    N93 --> N99
    N100[block]
    N93 --> N100
    N101(("{"))
    N100 --> N101
    N102[blockItem_list]
    N100 --> N102
    N103[blockItem_list]
    N102 --> N103
    N104[blockItem_list]
    N103 --> N104
    N105[blockItem_list]
    N104 --> N105
    N106[blockItem]
    N104 --> N106
    N107[decl]
    N106 --> N107
    N108[varDecl]
    N107 --> N108
    N109[bType]
    N108 --> N109
    N110(("int"))
    N109 --> N110
    N111[varDef]
    N108 --> N111
    N112(("a"))
    N111 --> N112
    N113(("="))
    N111 --> N113
    N114[initVal]
    N111 --> N114
    N115[exp]
    N114 --> N115
    N116[assignExp]
    N115 --> N116
    N117[lOrExp]
    N116 --> N117
    N118[lAndExp]
    N117 --> N118
    N119[eqExp]
    N118 --> N119
    N120[relExp]
    N119 --> N120
    N121[addExp]
    N120 --> N121
    N122[mulExp]
    N121 --> N122
    N123[unaryExp]
    N122 --> N123
    N124[primaryExp]
    N123 --> N124
    N125[number]
    N124 --> N125
    N126(("10"))
    N125 --> N126
    N127[varDef_list]
    N108 --> N127
    N128((";"))
    N108 --> N128
    N129[blockItem]
    N103 --> N129
    N130[decl]
    N129 --> N130
    N131[varDecl]
    N130 --> N131
    N132[bType]
    N131 --> N132
    N133(("int"))
    N132 --> N133
    N134[varDef]
    N131 --> N134
    N135(("b"))
    N134 --> N135
    N136(("="))
    N134 --> N136
    N137[initVal]
    N134 --> N137
    N138[exp]
    N137 --> N138
    N139[assignExp]
    N138 --> N139
    N140[lOrExp]
    N139 --> N140
    N141[lAndExp]
    N140 --> N141
    N142[eqExp]
    N141 --> N142
    N143[relExp]
    N142 --> N143
    N144[addExp]
    N143 --> N144
    N145[mulExp]
    N144 --> N145
    N146[unaryExp]
    N145 --> N146
    N147[primaryExp]
    N146 --> N147
    N148[number]
    N147 --> N148
    N149(("20"))
    N148 --> N149
    N150[varDef_list]
    N131 --> N150
    N151((";"))
    N131 --> N151
    N152[blockItem]
    N102 --> N152
    N153[stmt]
    N152 --> N153
    N154(("return"))
    N153 --> N154
    N155[exp_opt]
    N153 --> N155
    N156[exp]
    N155 --> N156
    N157[assignExp]
    N156 --> N157
    N158[lOrExp]
    N157 --> N158
    N159[lAndExp]
    N158 --> N159
    N160[eqExp]
    N159 --> N160
    N161[relExp]
    N160 --> N161
    N162[addExp]
    N161 --> N162
    N163[addExp]
    N162 --> N163
    N164[mulExp]
    N163 --> N164
    N165[unaryExp]
    N164 --> N165
    N166[primaryExp]
    N165 --> N166
    N167[lVal]
    N166 --> N167
    N168(("a"))
    N167 --> N168
    N169(("+"))
    N162 --> N169
    N170[mulExp]
    N162 --> N170
    N171[unaryExp]
    N170 --> N171
    N172[primaryExp]
    N171 --> N172
    N173[lVal]
    N172 --> N173
    N174(("b"))
    N173 --> N174
    N175((";"))
    N153 --> N175
    N176(("}"))
    N100 --> N176
    N177[compUnit_item]
    N4 --> N177
    N178[funcDef]
    N177 --> N178
    N179[bType]
    N178 --> N179
    N180(("int"))
    N179 --> N180
    N181(("test_mixed_expr"))
    N178 --> N181
    N182(("("))
    N178 --> N182
    N183[funcFParams_opt]
    N178 --> N183
    N184((")"))
    N178 --> N184
    N185[block]
    N178 --> N185
    N186(("{"))
    N185 --> N186
    N187[blockItem_list]
    N185 --> N187
    N188[blockItem_list]
    N187 --> N188
    N189[blockItem_list]
    N188 --> N189
    N190[blockItem_list]
    N189 --> N190
    N191[blockItem]
    N189 --> N191
    N192[decl]
    N191 --> N192
    N193[varDecl]
    N192 --> N193
    N194[bType]
    N193 --> N194
    N195(("float"))
    N194 --> N195
    N196[varDef]
    N193 --> N196
    N197(("a"))
    N196 --> N197
    N198(("="))
    N196 --> N198
    N199[initVal]
    N196 --> N199
    N200[exp]
    N199 --> N200
    N201[assignExp]
    N200 --> N201
    N202[lOrExp]
    N201 --> N202
    N203[lAndExp]
    N202 --> N203
    N204[eqExp]
    N203 --> N204
    N205[relExp]
    N204 --> N205
    N206[addExp]
    N205 --> N206
    N207[mulExp]
    N206 --> N207
    N208[unaryExp]
    N207 --> N208
    N209[primaryExp]
    N208 --> N209
    N210[number]
    N209 --> N210
    N211(("1.5"))
    N210 --> N211
    N212[varDef_list]
    N193 --> N212
    N213((";"))
    N193 --> N213
    N214[blockItem]
    N188 --> N214
    N215[decl]
    N214 --> N215
    N216[varDecl]
    N215 --> N216
    N217[bType]
    N216 --> N217
    N218(("int"))
    N217 --> N218
    N219[varDef]
    N216 --> N219
    N220(("b"))
    N219 --> N220
    N221(("="))
    N219 --> N221
    N222[initVal]
    N219 --> N222
    N223[exp]
    N222 --> N223
    N224[assignExp]
    N223 --> N224
    N225[lOrExp]
    N224 --> N225
    N226[lAndExp]
    N225 --> N226
    N227[eqExp]
    N226 --> N227
    N228[relExp]
    N227 --> N228
    N229[addExp]
    N228 --> N229
    N230[mulExp]
    N229 --> N230
    N231[unaryExp]
    N230 --> N231
    N232[primaryExp]
    N231 --> N232
    N233[number]
    N232 --> N233
    N234(("2"))
    N233 --> N234
    N235[varDef_list]
    N216 --> N235
    N236((";"))
    N216 --> N236
    N237[blockItem]
    N187 --> N237
    N238[stmt]
    N237 --> N238
    N239(("return"))
    N238 --> N239
    N240[exp_opt]
    N238 --> N240
    N241[exp]
    N240 --> N241
    N242[assignExp]
    N241 --> N242
    N243[lOrExp]
    N242 --> N243
    N244[lAndExp]
    N243 --> N244
    N245[eqExp]
    N244 --> N245
    N246[relExp]
    N245 --> N246
    N247[addExp]
    N246 --> N247
    N248[addExp]
    N247 --> N248
    N249[mulExp]
    N248 --> N249
    N250[unaryExp]
    N249 --> N250
    N251[primaryExp]
    N250 --> N251
    N252[lVal]
    N251 --> N252
    N253(("a"))
    N252 --> N253
    N254(("+"))
    N247 --> N254
    N255[mulExp]
    N247 --> N255
    N256[unaryExp]
    N255 --> N256
    N257[primaryExp]
    N256 --> N257
    N258[lVal]
    N257 --> N258
    N259(("b"))
    N258 --> N259
    N260((";"))
    N238 --> N260
    N261(("}"))
    N185 --> N261
    N262[compUnit_item]
    N3 --> N262
    N263[funcDef]
    N262 --> N263
    N264[bType]
    N263 --> N264
    N265(("float"))
    N264 --> N265
    N266(("test_mixed_to_float"))
    N263 --> N266
    N267(("("))
    N263 --> N267
    N268[funcFParams_opt]
    N263 --> N268
    N269((")"))
    N263 --> N269
    N270[block]
    N263 --> N270
    N271(("{"))
    N270 --> N271
    N272[blockItem_list]
    N270 --> N272
    N273[blockItem_list]
    N272 --> N273
    N274[blockItem_list]
    N273 --> N274
    N275[blockItem_list]
    N274 --> N275
    N276[blockItem_list]
    N275 --> N276
    N277[blockItem]
    N275 --> N277
    N278[decl]
    N277 --> N278
    N279[varDecl]
    N278 --> N279
    N280[bType]
    N279 --> N280
    N281(("float"))
    N280 --> N281
    N282[varDef]
    N279 --> N282
    N283(("a"))
    N282 --> N283
    N284(("="))
    N282 --> N284
    N285[initVal]
    N282 --> N285
    N286[exp]
    N285 --> N286
    N287[assignExp]
    N286 --> N287
    N288[lOrExp]
    N287 --> N288
    N289[lAndExp]
    N288 --> N289
    N290[eqExp]
    N289 --> N290
    N291[relExp]
    N290 --> N291
    N292[addExp]
    N291 --> N292
    N293[mulExp]
    N292 --> N293
    N294[unaryExp]
    N293 --> N294
    N295[primaryExp]
    N294 --> N295
    N296[number]
    N295 --> N296
    N297(("1.5"))
    N296 --> N297
    N298[varDef_list]
    N279 --> N298
    N299((";"))
    N279 --> N299
    N300[blockItem]
    N274 --> N300
    N301[decl]
    N300 --> N301
    N302[varDecl]
    N301 --> N302
    N303[bType]
    N302 --> N303
    N304(("int"))
    N303 --> N304
    N305[varDef]
    N302 --> N305
    N306(("b"))
    N305 --> N306
    N307(("="))
    N305 --> N307
    N308[initVal]
    N305 --> N308
    N309[exp]
    N308 --> N309
    N310[assignExp]
    N309 --> N310
    N311[lOrExp]
    N310 --> N311
    N312[lAndExp]
    N311 --> N312
    N313[eqExp]
    N312 --> N313
    N314[relExp]
    N313 --> N314
    N315[addExp]
    N314 --> N315
    N316[mulExp]
    N315 --> N316
    N317[unaryExp]
    N316 --> N317
    N318[primaryExp]
    N317 --> N318
    N319[number]
    N318 --> N319
    N320(("2"))
    N319 --> N320
    N321[varDef_list]
    N302 --> N321
    N322((";"))
    N302 --> N322
    N323[blockItem]
    N273 --> N323
    N324[decl]
    N323 --> N324
    N325[varDecl]
    N324 --> N325
    N326[bType]
    N325 --> N326
    N327(("float"))
    N326 --> N327
    N328[varDef]
    N325 --> N328
    N329(("c"))
    N328 --> N329
    N330(("="))
    N328 --> N330
    N331[initVal]
    N328 --> N331
    N332[exp]
    N331 --> N332
    N333[assignExp]
    N332 --> N333
    N334[lOrExp]
    N333 --> N334
    N335[lAndExp]
    N334 --> N335
    N336[eqExp]
    N335 --> N336
    N337[relExp]
    N336 --> N337
    N338[addExp]
    N337 --> N338
    N339[addExp]
    N338 --> N339
    N340[mulExp]
    N339 --> N340
    N341[unaryExp]
    N340 --> N341
    N342[primaryExp]
    N341 --> N342
    N343[lVal]
    N342 --> N343
    N344(("a"))
    N343 --> N344
    N345(("+"))
    N338 --> N345
    N346[mulExp]
    N338 --> N346
    N347[unaryExp]
    N346 --> N347
    N348[primaryExp]
    N347 --> N348
    N349[lVal]
    N348 --> N349
    N350(("b"))
    N349 --> N350
    N351[varDef_list]
    N325 --> N351
    N352((";"))
    N325 --> N352
    N353[blockItem]
    N272 --> N353
    N354[stmt]
    N353 --> N354
    N355(("return"))
    N354 --> N355
    N356[exp_opt]
    N354 --> N356
    N357[exp]
    N356 --> N357
    N358[assignExp]
    N357 --> N358
    N359[lOrExp]
    N358 --> N359
    N360[lAndExp]
    N359 --> N360
    N361[eqExp]
    N360 --> N361
    N362[relExp]
    N361 --> N362
    N363[addExp]
    N362 --> N363
    N364[mulExp]
    N363 --> N364
    N365[unaryExp]
    N364 --> N365
    N366[primaryExp]
    N365 --> N366
    N367[lVal]
    N366 --> N367
    N368(("c"))
    N367 --> N368
    N369((";"))
    N354 --> N369
    N370(("}"))
    N270 --> N370
    N371[compUnit_item]
    N2 --> N371
    N372[funcDef]
    N371 --> N372
    N373[bType]
    N372 --> N373
    N374(("int"))
    N373 --> N374
    N375(("main"))
    N372 --> N375
    N376(("("))
    N372 --> N376
    N377[funcFParams_opt]
    N372 --> N377
    N378((")"))
    N372 --> N378
    N379[block]
    N372 --> N379
    N380(("{"))
    N379 --> N380
    N381[blockItem_list]
    N379 --> N381
    N382[blockItem_list]
    N381 --> N382
    N383[blockItem_list]
    N382 --> N383
    N384[blockItem_list]
    N383 --> N384
    N385[blockItem_list]
    N384 --> N385
    N386[blockItem_list]
    N385 --> N386
    N387[blockItem]
    N385 --> N387
    N388[stmt]
    N387 --> N388
    N389[exp_opt]
    N388 --> N389
    N390[exp]
    N389 --> N390
    N391[assignExp]
    N390 --> N391
    N392[lOrExp]
    N391 --> N392
    N393[lAndExp]
    N392 --> N393
    N394[eqExp]
    N393 --> N394
    N395[relExp]
    N394 --> N395
    N396[addExp]
    N395 --> N396
    N397[mulExp]
    N396 --> N397
    N398[unaryExp]
    N397 --> N398
    N399(("test_float_add"))
    N398 --> N399
    N400(("("))
    N398 --> N400
    N401[funcRParams_opt]
    N398 --> N401
    N402((")"))
    N398 --> N402
    N403((";"))
    N388 --> N403
    N404[blockItem]
    N384 --> N404
    N405[stmt]
    N404 --> N405
    N406[exp_opt]
    N405 --> N406
    N407[exp]
    N406 --> N407
    N408[assignExp]
    N407 --> N408
    N409[lOrExp]
    N408 --> N409
    N410[lAndExp]
    N409 --> N410
    N411[eqExp]
    N410 --> N411
    N412[relExp]
    N411 --> N412
    N413[addExp]
    N412 --> N413
    N414[mulExp]
    N413 --> N414
    N415[unaryExp]
    N414 --> N415
    N416(("test_int_add"))
    N415 --> N416
    N417(("("))
    N415 --> N417
    N418[funcRParams_opt]
    N415 --> N418
    N419((")"))
    N415 --> N419
    N420((";"))
    N405 --> N420
    N421[blockItem]
    N383 --> N421
    N422[stmt]
    N421 --> N422
    N423[exp_opt]
    N422 --> N423
    N424[exp]
    N423 --> N424
    N425[assignExp]
    N424 --> N425
    N426[lOrExp]
    N425 --> N426
    N427[lAndExp]
    N426 --> N427
    N428[eqExp]
    N427 --> N428
    N429[relExp]
    N428 --> N429
    N430[addExp]
    N429 --> N430
    N431[mulExp]
    N430 --> N431
    N432[unaryExp]
    N431 --> N432
    N433(("test_mixed_expr"))
    N432 --> N433
    N434(("("))
    N432 --> N434
    N435[funcRParams_opt]
    N432 --> N435
    N436((")"))
    N432 --> N436
    N437((";"))
    N422 --> N437
    N438[blockItem]
    N382 --> N438
    N439[stmt]
    N438 --> N439
    N440[exp_opt]
    N439 --> N440
    N441[exp]
    N440 --> N441
    N442[assignExp]
    N441 --> N442
    N443[lOrExp]
    N442 --> N443
    N444[lAndExp]
    N443 --> N444
    N445[eqExp]
    N444 --> N445
    N446[relExp]
    N445 --> N446
    N447[addExp]
    N446 --> N447
    N448[mulExp]
    N447 --> N448
    N449[unaryExp]
    N448 --> N449
    N450(("test_mixed_to_float"))
    N449 --> N450
    N451(("("))
    N449 --> N451
    N452[funcRParams_opt]
    N449 --> N452
    N453((")"))
    N449 --> N453
    N454((";"))
    N439 --> N454
    N455[blockItem]
    N381 --> N455
    N456[stmt]
    N455 --> N456
    N457(("return"))
    N456 --> N457
    N458[exp_opt]
    N456 --> N458
    N459[exp]
    N458 --> N459
    N460[assignExp]
    N459 --> N460
    N461[lOrExp]
    N460 --> N461
    N462[lAndExp]
    N461 --> N462
    N463[eqExp]
    N462 --> N463
    N464[relExp]
    N463 --> N464
    N465[addExp]
    N464 --> N465
    N466[mulExp]
    N465 --> N466
    N467[unaryExp]
    N466 --> N467
    N468[primaryExp]
    N467 --> N468
    N469[number]
    N468 --> N469
    N470(("0"))
    N469 --> N470
    N471((";"))
    N456 --> N471
    N472(("}"))
    N379 --> N472
    N473(("}"))
    N1 --> N473
```
