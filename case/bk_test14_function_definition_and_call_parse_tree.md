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
    N4 --> N5
    N6[funcDef]
    N5 --> N6
    N7[bType]
    N6 --> N7
    N8(("int"))
    N7 --> N8
    N9(("add"))
    N6 --> N9
    N10(("("))
    N6 --> N10
    N11[funcFParams_opt]
    N6 --> N11
    N12[funcFParams]
    N11 --> N12
    N13[funcFParam]
    N12 --> N13
    N14[bType]
    N13 --> N14
    N15(("int"))
    N14 --> N15
    N16(("num1"))
    N13 --> N16
    N17[funcFParam_list]
    N12 --> N17
    N18[funcFParam_list]
    N17 --> N18
    N19[funcFParam_list]
    N18 --> N19
    N20((","))
    N18 --> N20
    N21[funcFParam]
    N18 --> N21
    N22[bType]
    N21 --> N22
    N23(("int"))
    N22 --> N23
    N24(("num2"))
    N21 --> N24
    N25((","))
    N17 --> N25
    N26[funcFParam]
    N17 --> N26
    N27[bType]
    N26 --> N27
    N28(("int"))
    N27 --> N28
    N29(("mod"))
    N26 --> N29
    N30((")"))
    N6 --> N30
    N31[block]
    N6 --> N31
    N32(("{"))
    N31 --> N32
    N33[blockItem_list]
    N31 --> N33
    N34[blockItem_list]
    N33 --> N34
    N35[blockItem]
    N33 --> N35
    N36[stmt]
    N35 --> N36
    N37(("return"))
    N36 --> N37
    N38[exp_opt]
    N36 --> N38
    N39[exp]
    N38 --> N39
    N40[assignExp]
    N39 --> N40
    N41[lOrExp]
    N40 --> N41
    N42[lAndExp]
    N41 --> N42
    N43[eqExp]
    N42 --> N43
    N44[relExp]
    N43 --> N44
    N45[addExp]
    N44 --> N45
    N46[mulExp]
    N45 --> N46
    N47[mulExp]
    N46 --> N47
    N48[unaryExp]
    N47 --> N48
    N49[primaryExp]
    N48 --> N49
    N50(("("))
    N49 --> N50
    N51[exp]
    N49 --> N51
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
    N58[addExp]
    N57 --> N58
    N59[mulExp]
    N58 --> N59
    N60[unaryExp]
    N59 --> N60
    N61[primaryExp]
    N60 --> N61
    N62[lVal]
    N61 --> N62
    N63(("num1"))
    N62 --> N63
    N64(("+"))
    N57 --> N64
    N65[mulExp]
    N57 --> N65
    N66[unaryExp]
    N65 --> N66
    N67[primaryExp]
    N66 --> N67
    N68[lVal]
    N67 --> N68
    N69(("num2"))
    N68 --> N69
    N70((")"))
    N49 --> N70
    N71(("%"))
    N46 --> N71
    N72[unaryExp]
    N46 --> N72
    N73[primaryExp]
    N72 --> N73
    N74[lVal]
    N73 --> N74
    N75(("mod"))
    N74 --> N75
    N76((";"))
    N36 --> N76
    N77(("}"))
    N31 --> N77
    N78[compUnit_item]
    N3 --> N78
    N79[funcDef]
    N78 --> N79
    N80[bType]
    N79 --> N80
    N81(("void"))
    N80 --> N81
    N82(("test"))
    N79 --> N82
    N83(("("))
    N79 --> N83
    N84[funcFParams_opt]
    N79 --> N84
    N85((")"))
    N79 --> N85
    N86[block]
    N79 --> N86
    N87(("{"))
    N86 --> N87
    N88[blockItem_list]
    N86 --> N88
    N89(("}"))
    N86 --> N89
    N90[compUnit_item]
    N2 --> N90
    N91[funcDef]
    N90 --> N91
    N92[bType]
    N91 --> N92
    N93(("void"))
    N92 --> N93
    N94(("main"))
    N91 --> N94
    N95(("("))
    N91 --> N95
    N96[funcFParams_opt]
    N91 --> N96
    N97((")"))
    N91 --> N97
    N98[block]
    N91 --> N98
    N99(("{"))
    N98 --> N99
    N100[blockItem_list]
    N98 --> N100
    N101[blockItem_list]
    N100 --> N101
    N102[blockItem_list]
    N101 --> N102
    N103[blockItem_list]
    N102 --> N103
    N104[blockItem_list]
    N103 --> N104
    N105[blockItem_list]
    N104 --> N105
    N106[blockItem_list]
    N105 --> N106
    N107[blockItem_list]
    N106 --> N107
    N108[blockItem_list]
    N107 --> N108
    N109[blockItem_list]
    N108 --> N109
    N110[blockItem_list]
    N109 --> N110
    N111[blockItem_list]
    N110 --> N111
    N112[blockItem_list]
    N111 --> N112
    N113[blockItem]
    N111 --> N113
    N114[decl]
    N113 --> N114
    N115[varDecl]
    N114 --> N115
    N116[bType]
    N115 --> N116
    N117(("int"))
    N116 --> N117
    N118[varDef]
    N115 --> N118
    N119(("a"))
    N118 --> N119
    N120[varDef_list]
    N115 --> N120
    N121((";"))
    N115 --> N121
    N122[blockItem]
    N110 --> N122
    N123[decl]
    N122 --> N123
    N124[varDecl]
    N123 --> N124
    N125[bType]
    N124 --> N125
    N126(("int"))
    N125 --> N126
    N127[varDef]
    N124 --> N127
    N128(("b"))
    N127 --> N128
    N129(("="))
    N127 --> N129
    N130[initVal]
    N127 --> N130
    N131[exp]
    N130 --> N131
    N132[assignExp]
    N131 --> N132
    N133[lOrExp]
    N132 --> N133
    N134[lAndExp]
    N133 --> N134
    N135[eqExp]
    N134 --> N135
    N136[relExp]
    N135 --> N136
    N137[addExp]
    N136 --> N137
    N138[mulExp]
    N137 --> N138
    N139[unaryExp]
    N138 --> N139
    N140[primaryExp]
    N139 --> N140
    N141[number]
    N140 --> N141
    N142(("2"))
    N141 --> N142
    N143[varDef_list]
    N124 --> N143
    N144((";"))
    N124 --> N144
    N145[blockItem]
    N109 --> N145
    N146[decl]
    N145 --> N146
    N147[varDecl]
    N146 --> N147
    N148[bType]
    N147 --> N148
    N149(("int"))
    N148 --> N149
    N150[varDef]
    N147 --> N150
    N151(("c"))
    N150 --> N151
    N152(("="))
    N150 --> N152
    N153[initVal]
    N150 --> N153
    N154[exp]
    N153 --> N154
    N155[assignExp]
    N154 --> N155
    N156[lOrExp]
    N155 --> N156
    N157[lAndExp]
    N156 --> N157
    N158[eqExp]
    N157 --> N158
    N159[relExp]
    N158 --> N159
    N160[addExp]
    N159 --> N160
    N161[mulExp]
    N160 --> N161
    N162[unaryExp]
    N161 --> N162
    N163[unaryOp]
    N162 --> N163
    N164(("-"))
    N163 --> N164
    N165[unaryExp]
    N162 --> N165
    N166[primaryExp]
    N165 --> N166
    N167[number]
    N166 --> N167
    N168(("32"))
    N167 --> N168
    N169[varDef_list]
    N147 --> N169
    N170((";"))
    N147 --> N170
    N171[blockItem]
    N108 --> N171
    N172[decl]
    N171 --> N172
    N173[varDecl]
    N172 --> N173
    N174[bType]
    N173 --> N174
    N175(("float"))
    N174 --> N175
    N176[varDef]
    N173 --> N176
    N177(("f1"))
    N176 --> N177
    N178(("="))
    N176 --> N178
    N179[initVal]
    N176 --> N179
    N180[exp]
    N179 --> N180
    N181[assignExp]
    N180 --> N181
    N182[lOrExp]
    N181 --> N182
    N183[lAndExp]
    N182 --> N183
    N184[eqExp]
    N183 --> N184
    N185[relExp]
    N184 --> N185
    N186[addExp]
    N185 --> N186
    N187[mulExp]
    N186 --> N187
    N188[unaryExp]
    N187 --> N188
    N189[primaryExp]
    N188 --> N189
    N190[number]
    N189 --> N190
    N191(("1.32"))
    N190 --> N191
    N192[varDef_list]
    N173 --> N192
    N193((";"))
    N173 --> N193
    N194[blockItem]
    N107 --> N194
    N195[decl]
    N194 --> N195
    N196[varDecl]
    N195 --> N196
    N197[bType]
    N196 --> N197
    N198(("float"))
    N197 --> N198
    N199[varDef]
    N196 --> N199
    N200(("f2"))
    N199 --> N200
    N201(("="))
    N199 --> N201
    N202[initVal]
    N199 --> N202
    N203[exp]
    N202 --> N203
    N204[assignExp]
    N203 --> N204
    N205[lOrExp]
    N204 --> N205
    N206[lAndExp]
    N205 --> N206
    N207[eqExp]
    N206 --> N207
    N208[relExp]
    N207 --> N208
    N209[addExp]
    N208 --> N209
    N210[mulExp]
    N209 --> N210
    N211[unaryExp]
    N210 --> N211
    N212[unaryOp]
    N211 --> N212
    N213(("-"))
    N212 --> N213
    N214[unaryExp]
    N211 --> N214
    N215[primaryExp]
    N214 --> N215
    N216[number]
    N215 --> N216
    N217(("3123.3"))
    N216 --> N217
    N218[varDef_list]
    N196 --> N218
    N219((";"))
    N196 --> N219
    N220[blockItem]
    N106 --> N220
    N221[stmt]
    N220 --> N221
    N222[exp_opt]
    N221 --> N222
    N223[exp]
    N222 --> N223
    N224[assignExp]
    N223 --> N224
    N225[lVal]
    N224 --> N225
    N226(("a"))
    N225 --> N226
    N227(("="))
    N224 --> N227
    N228[assignExp]
    N224 --> N228
    N229[lOrExp]
    N228 --> N229
    N230[lAndExp]
    N229 --> N230
    N231[eqExp]
    N230 --> N231
    N232[relExp]
    N231 --> N232
    N233[addExp]
    N232 --> N233
    N234[mulExp]
    N233 --> N234
    N235[unaryExp]
    N234 --> N235
    N236[primaryExp]
    N235 --> N236
    N237[number]
    N236 --> N237
    N238(("32"))
    N237 --> N238
    N239((";"))
    N221 --> N239
    N240[blockItem]
    N105 --> N240
    N241[stmt]
    N240 --> N241
    N242[exp_opt]
    N241 --> N242
    N243[exp]
    N242 --> N243
    N244[assignExp]
    N243 --> N244
    N245[lVal]
    N244 --> N245
    N246(("b"))
    N245 --> N246
    N247(("="))
    N244 --> N247
    N248[assignExp]
    N244 --> N248
    N249[lOrExp]
    N248 --> N249
    N250[lAndExp]
    N249 --> N250
    N251[eqExp]
    N250 --> N251
    N252[relExp]
    N251 --> N252
    N253[addExp]
    N252 --> N253
    N254[addExp]
    N253 --> N254
    N255[mulExp]
    N254 --> N255
    N256[unaryExp]
    N255 --> N256
    N257[primaryExp]
    N256 --> N257
    N258[lVal]
    N257 --> N258
    N259(("a"))
    N258 --> N259
    N260(("+"))
    N253 --> N260
    N261[mulExp]
    N253 --> N261
    N262[unaryExp]
    N261 --> N262
    N263[primaryExp]
    N262 --> N263
    N264[number]
    N263 --> N264
    N265(("1"))
    N264 --> N265
    N266((";"))
    N241 --> N266
    N267[blockItem]
    N104 --> N267
    N268[decl]
    N267 --> N268
    N269[constDecl]
    N268 --> N269
    N270(("const"))
    N269 --> N270
    N271[bType]
    N269 --> N271
    N272(("int"))
    N271 --> N272
    N273[constDef]
    N269 --> N273
    N274(("g"))
    N273 --> N274
    N275(("="))
    N273 --> N275
    N276[constInitVal]
    N273 --> N276
    N277[constExp]
    N276 --> N277
    N278[addExp]
    N277 --> N278
    N279[addExp]
    N278 --> N279
    N280[mulExp]
    N279 --> N280
    N281[unaryExp]
    N280 --> N281
    N282[unaryOp]
    N281 --> N282
    N283(("-"))
    N282 --> N283
    N284[unaryExp]
    N281 --> N284
    N285[primaryExp]
    N284 --> N285
    N286[lVal]
    N285 --> N286
    N287(("b"))
    N286 --> N287
    N288(("+"))
    N278 --> N288
    N289[mulExp]
    N278 --> N289
    N290[mulExp]
    N289 --> N290
    N291[unaryExp]
    N290 --> N291
    N292[primaryExp]
    N291 --> N292
    N293(("("))
    N292 --> N293
    N294[exp]
    N292 --> N294
    N295[assignExp]
    N294 --> N295
    N296[lOrExp]
    N295 --> N296
    N297[lAndExp]
    N296 --> N297
    N298[eqExp]
    N297 --> N298
    N299[relExp]
    N298 --> N299
    N300[addExp]
    N299 --> N300
    N301[addExp]
    N300 --> N301
    N302[mulExp]
    N301 --> N302
    N303[unaryExp]
    N302 --> N303
    N304[primaryExp]
    N303 --> N304
    N305[lVal]
    N304 --> N305
    N306(("a"))
    N305 --> N306
    N307(("+"))
    N300 --> N307
    N308[mulExp]
    N300 --> N308
    N309[unaryExp]
    N308 --> N309
    N310[primaryExp]
    N309 --> N310
    N311[lVal]
    N310 --> N311
    N312(("c"))
    N311 --> N312
    N313((")"))
    N292 --> N313
    N314(("*"))
    N289 --> N314
    N315[unaryExp]
    N289 --> N315
    N316[primaryExp]
    N315 --> N316
    N317(("("))
    N316 --> N317
    N318[exp]
    N316 --> N318
    N319[assignExp]
    N318 --> N319
    N320[lOrExp]
    N319 --> N320
    N321[lAndExp]
    N320 --> N321
    N322[eqExp]
    N321 --> N322
    N323[relExp]
    N322 --> N323
    N324[addExp]
    N323 --> N324
    N325[mulExp]
    N324 --> N325
    N326[unaryExp]
    N325 --> N326
    N327[unaryOp]
    N326 --> N327
    N328(("-"))
    N327 --> N328
    N329[unaryExp]
    N326 --> N329
    N330[primaryExp]
    N329 --> N330
    N331[lVal]
    N330 --> N331
    N332(("a"))
    N331 --> N332
    N333((")"))
    N316 --> N333
    N334[constDef_list]
    N269 --> N334
    N335((";"))
    N269 --> N335
    N336[blockItem]
    N103 --> N336
    N337[stmt]
    N336 --> N337
    N338(("if"))
    N337 --> N338
    N339(("("))
    N337 --> N339
    N340[cond]
    N337 --> N340
    N341[lOrExp]
    N340 --> N341
    N342[lAndExp]
    N341 --> N342
    N343[lAndExp]
    N342 --> N343
    N344[eqExp]
    N343 --> N344
    N345[relExp]
    N344 --> N345
    N346[addExp]
    N345 --> N346
    N347[mulExp]
    N346 --> N347
    N348[unaryExp]
    N347 --> N348
    N349[unaryOp]
    N348 --> N349
    N350(("!"))
    N349 --> N350
    N351[unaryExp]
    N348 --> N351
    N352[primaryExp]
    N351 --> N352
    N353[number]
    N352 --> N353
    N354(("0"))
    N353 --> N354
    N355(("&&"))
    N342 --> N355
    N356[eqExp]
    N342 --> N356
    N357[relExp]
    N356 --> N357
    N358[addExp]
    N357 --> N358
    N359[mulExp]
    N358 --> N359
    N360[unaryExp]
    N359 --> N360
    N361[primaryExp]
    N360 --> N361
    N362[number]
    N361 --> N362
    N363(("1"))
    N362 --> N363
    N364((")"))
    N337 --> N364
    N365[stmt]
    N337 --> N365
    N366[block]
    N365 --> N366
    N367(("{"))
    N366 --> N367
    N368[blockItem_list]
    N366 --> N368
    N369[blockItem_list]
    N368 --> N369
    N370[blockItem_list]
    N369 --> N370
    N371[blockItem]
    N369 --> N371
    N372[stmt]
    N371 --> N372
    N373[exp_opt]
    N372 --> N373
    N374[exp]
    N373 --> N374
    N375[assignExp]
    N374 --> N375
    N376[lVal]
    N375 --> N376
    N377(("a"))
    N376 --> N377
    N378(("="))
    N375 --> N378
    N379[assignExp]
    N375 --> N379
    N380[lOrExp]
    N379 --> N380
    N381[lAndExp]
    N380 --> N381
    N382[eqExp]
    N381 --> N382
    N383[relExp]
    N382 --> N383
    N384[addExp]
    N383 --> N384
    N385[mulExp]
    N384 --> N385
    N386[unaryExp]
    N385 --> N386
    N387[primaryExp]
    N386 --> N387
    N388[number]
    N387 --> N388
    N389(("2"))
    N388 --> N389
    N390((";"))
    N372 --> N390
    N391[blockItem]
    N368 --> N391
    N392[stmt]
    N391 --> N392
    N393[exp_opt]
    N392 --> N393
    N394[exp]
    N393 --> N394
    N395[assignExp]
    N394 --> N395
    N396[lVal]
    N395 --> N396
    N397(("g"))
    N396 --> N397
    N398(("="))
    N395 --> N398
    N399[assignExp]
    N395 --> N399
    N400[lOrExp]
    N399 --> N400
    N401[lAndExp]
    N400 --> N401
    N402[eqExp]
    N401 --> N402
    N403[relExp]
    N402 --> N403
    N404[addExp]
    N403 --> N404
    N405[mulExp]
    N404 --> N405
    N406[unaryExp]
    N405 --> N406
    N407[primaryExp]
    N406 --> N407
    N408[number]
    N407 --> N408
    N409(("1"))
    N408 --> N409
    N410((";"))
    N392 --> N410
    N411(("}"))
    N366 --> N411
    N412[else_opt]
    N337 --> N412
    N413[blockItem]
    N102 --> N413
    N414[decl]
    N413 --> N414
    N415[varDecl]
    N414 --> N415
    N416[bType]
    N415 --> N416
    N417(("int"))
    N416 --> N417
    N418[varDef]
    N415 --> N418
    N419(("ret"))
    N418 --> N419
    N420(("="))
    N418 --> N420
    N421[initVal]
    N418 --> N421
    N422[exp]
    N421 --> N422
    N423[assignExp]
    N422 --> N423
    N424[lOrExp]
    N423 --> N424
    N425[lAndExp]
    N424 --> N425
    N426[eqExp]
    N425 --> N426
    N427[relExp]
    N426 --> N427
    N428[addExp]
    N427 --> N428
    N429[mulExp]
    N428 --> N429
    N430[unaryExp]
    N429 --> N430
    N431(("add"))
    N430 --> N431
    N432(("("))
    N430 --> N432
    N433[funcRParams_opt]
    N430 --> N433
    N434[funcRParams]
    N433 --> N434
    N435[funcRParam]
    N434 --> N435
    N436[exp]
    N435 --> N436
    N437[assignExp]
    N436 --> N437
    N438[lOrExp]
    N437 --> N438
    N439[lAndExp]
    N438 --> N439
    N440[eqExp]
    N439 --> N440
    N441[relExp]
    N440 --> N441
    N442[addExp]
    N441 --> N442
    N443[mulExp]
    N442 --> N443
    N444[unaryExp]
    N443 --> N444
    N445[primaryExp]
    N444 --> N445
    N446[lVal]
    N445 --> N446
    N447(("b"))
    N446 --> N447
    N448[funcRParam_list]
    N434 --> N448
    N449[funcRParam_list]
    N448 --> N449
    N450((","))
    N448 --> N450
    N451[funcRParam]
    N448 --> N451
    N452[exp]
    N451 --> N452
    N453[assignExp]
    N452 --> N453
    N454[lOrExp]
    N453 --> N454
    N455[lAndExp]
    N454 --> N455
    N456[eqExp]
    N455 --> N456
    N457[relExp]
    N456 --> N457
    N458[addExp]
    N457 --> N458
    N459[mulExp]
    N458 --> N459
    N460[unaryExp]
    N459 --> N460
    N461[primaryExp]
    N460 --> N461
    N462[lVal]
    N461 --> N462
    N463(("c"))
    N462 --> N463
    N464((")"))
    N430 --> N464
    N465[varDef_list]
    N415 --> N465
    N466((";"))
    N415 --> N466
    N467[blockItem]
    N101 --> N467
    N468[stmt]
    N467 --> N468
    N469[exp_opt]
    N468 --> N469
    N470[exp]
    N469 --> N470
    N471[assignExp]
    N470 --> N471
    N472[lOrExp]
    N471 --> N472
    N473[lAndExp]
    N472 --> N473
    N474[eqExp]
    N473 --> N474
    N475[relExp]
    N474 --> N475
    N476[addExp]
    N475 --> N476
    N477[mulExp]
    N476 --> N477
    N478[unaryExp]
    N477 --> N478
    N479(("test"))
    N478 --> N479
    N480(("("))
    N478 --> N480
    N481[funcRParams_opt]
    N478 --> N481
    N482((")"))
    N478 --> N482
    N483((";"))
    N468 --> N483
    N484[blockItem]
    N100 --> N484
    N485[stmt]
    N484 --> N485
    N486(("return"))
    N485 --> N486
    N487[exp_opt]
    N485 --> N487
    N488((";"))
    N485 --> N488
    N489(("}"))
    N98 --> N489
    N490(("}"))
    N1 --> N490
```
