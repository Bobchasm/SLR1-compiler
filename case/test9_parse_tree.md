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
    N7(("_"))
    N4 --> N7
    N8(("("))
    N4 --> N8
    N9[funcFParams_opt]
    N4 --> N9
    N10[funcFParams]
    N9 --> N10
    N11[funcFParam]
    N10 --> N11
    N12[bType]
    N11 --> N12
    N13(("int"))
    N12 --> N13
    N14(("_1"))
    N11 --> N14
    N15[funcFParam_list]
    N10 --> N15
    N16[funcFParam_list]
    N15 --> N16
    N17[funcFParam_list]
    N16 --> N17
    N18((","))
    N16 --> N18
    N19[funcFParam]
    N16 --> N19
    N20[bType]
    N19 --> N20
    N21(("int"))
    N20 --> N21
    N22(("_2"))
    N19 --> N22
    N23((","))
    N15 --> N23
    N24[funcFParam]
    N15 --> N24
    N25[bType]
    N24 --> N25
    N26(("int"))
    N25 --> N26
    N27(("_3"))
    N24 --> N27
    N28((")"))
    N4 --> N28
    N29[block]
    N4 --> N29
    N30(("{"))
    N29 --> N30
    N31[blockItem_list]
    N29 --> N31
    N32[blockItem_list]
    N31 --> N32
    N33[blockItem_list]
    N32 --> N33
    N34[blockItem_list]
    N33 --> N34
    N35[blockItem_list]
    N34 --> N35
    N36[blockItem_list]
    N35 --> N36
    N37[blockItem]
    N35 --> N37
    N38[decl]
    N37 --> N38
    N39[varDecl]
    N38 --> N39
    N40[bType]
    N39 --> N40
    N41(("int"))
    N40 --> N41
    N42[varDef]
    N39 --> N42
    N43(("_a"))
    N42 --> N43
    N44(("="))
    N42 --> N44
    N45[initVal]
    N42 --> N45
    N46[exp]
    N45 --> N46
    N47[assignExp]
    N46 --> N47
    N48[lOrExp]
    N47 --> N48
    N49[lAndExp]
    N48 --> N49
    N50[eqExp]
    N49 --> N50
    N51[relExp]
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
    N57(("0"))
    N56 --> N57
    N58[varDef_list]
    N39 --> N58
    N59((";"))
    N39 --> N59
    N60[blockItem]
    N34 --> N60
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
    N66(("_b"))
    N65 --> N66
    N67(("="))
    N65 --> N67
    N68[initVal]
    N65 --> N68
    N69[exp]
    N68 --> N69
    N70[assignExp]
    N69 --> N70
    N71[lOrExp]
    N70 --> N71
    N72[lAndExp]
    N71 --> N72
    N73[eqExp]
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
    N79[number]
    N78 --> N79
    N80(("0"))
    N79 --> N80
    N81[varDef_list]
    N62 --> N81
    N82((";"))
    N62 --> N82
    N83[blockItem]
    N33 --> N83
    N84[decl]
    N83 --> N84
    N85[varDecl]
    N84 --> N85
    N86[bType]
    N85 --> N86
    N87(("int"))
    N86 --> N87
    N88[varDef]
    N85 --> N88
    N89(("_c"))
    N88 --> N89
    N90(("="))
    N88 --> N90
    N91[initVal]
    N88 --> N91
    N92[exp]
    N91 --> N92
    N93[assignExp]
    N92 --> N93
    N94[lOrExp]
    N93 --> N94
    N95[lAndExp]
    N94 --> N95
    N96[eqExp]
    N95 --> N96
    N97[relExp]
    N96 --> N97
    N98[addExp]
    N97 --> N98
    N99[mulExp]
    N98 --> N99
    N100[unaryExp]
    N99 --> N100
    N101[primaryExp]
    N100 --> N101
    N102[number]
    N101 --> N102
    N103(("0"))
    N102 --> N103
    N104[varDef_list]
    N85 --> N104
    N105((";"))
    N85 --> N105
    N106[blockItem]
    N32 --> N106
    N107[stmt]
    N106 --> N107
    N108(("if"))
    N107 --> N108
    N109(("("))
    N107 --> N109
    N110[cond]
    N107 --> N110
    N111[lOrExp]
    N110 --> N111
    N112[lOrExp]
    N111 --> N112
    N113[lAndExp]
    N112 --> N113
    N114[eqExp]
    N113 --> N114
    N115[relExp]
    N114 --> N115
    N116[addExp]
    N115 --> N116
    N117[mulExp]
    N116 --> N117
    N118[unaryExp]
    N117 --> N118
    N119[primaryExp]
    N118 --> N119
    N120(("("))
    N119 --> N120
    N121[exp]
    N119 --> N121
    N122[assignExp]
    N121 --> N122
    N123[lOrExp]
    N122 --> N123
    N124[lAndExp]
    N123 --> N124
    N125[lAndExp]
    N124 --> N125
    N126[eqExp]
    N125 --> N126
    N127[relExp]
    N126 --> N127
    N128[relExp]
    N127 --> N128
    N129[addExp]
    N128 --> N129
    N130[mulExp]
    N129 --> N130
    N131[unaryExp]
    N130 --> N131
    N132[primaryExp]
    N131 --> N132
    N133(("("))
    N132 --> N133
    N134[exp]
    N132 --> N134
    N135[assignExp]
    N134 --> N135
    N136[lVal]
    N135 --> N136
    N137(("_a"))
    N136 --> N137
    N138(("="))
    N135 --> N138
    N139[assignExp]
    N135 --> N139
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
    N145[addExp]
    N144 --> N145
    N146[mulExp]
    N145 --> N146
    N147[unaryExp]
    N146 --> N147
    N148[primaryExp]
    N147 --> N148
    N149[lVal]
    N148 --> N149
    N150(("_1"))
    N149 --> N150
    N151(("+"))
    N144 --> N151
    N152[mulExp]
    N144 --> N152
    N153[unaryExp]
    N152 --> N153
    N154[primaryExp]
    N153 --> N154
    N155[number]
    N154 --> N155
    N156(("1"))
    N155 --> N156
    N157((")"))
    N132 --> N157
    N158((">"))
    N127 --> N158
    N159[addExp]
    N127 --> N159
    N160[mulExp]
    N159 --> N160
    N161[unaryExp]
    N160 --> N161
    N162[primaryExp]
    N161 --> N162
    N163[number]
    N162 --> N163
    N164(("0"))
    N163 --> N164
    N165(("&&"))
    N124 --> N165
    N166[eqExp]
    N124 --> N166
    N167[relExp]
    N166 --> N167
    N168[addExp]
    N167 --> N168
    N169[mulExp]
    N168 --> N169
    N170[unaryExp]
    N169 --> N170
    N171[primaryExp]
    N170 --> N171
    N172(("("))
    N171 --> N172
    N173[exp]
    N171 --> N173
    N174[assignExp]
    N173 --> N174
    N175[lOrExp]
    N174 --> N175
    N176[lAndExp]
    N175 --> N176
    N177[eqExp]
    N176 --> N177
    N178[relExp]
    N177 --> N178
    N179[relExp]
    N178 --> N179
    N180[addExp]
    N179 --> N180
    N181[mulExp]
    N180 --> N181
    N182[unaryExp]
    N181 --> N182
    N183[primaryExp]
    N182 --> N183
    N184(("("))
    N183 --> N184
    N185[exp]
    N183 --> N185
    N186[assignExp]
    N185 --> N186
    N187[lVal]
    N186 --> N187
    N188(("_b"))
    N187 --> N188
    N189(("="))
    N186 --> N189
    N190[assignExp]
    N186 --> N190
    N191[lOrExp]
    N190 --> N191
    N192[lAndExp]
    N191 --> N192
    N193[eqExp]
    N192 --> N193
    N194[relExp]
    N193 --> N194
    N195[addExp]
    N194 --> N195
    N196[addExp]
    N195 --> N196
    N197[mulExp]
    N196 --> N197
    N198[unaryExp]
    N197 --> N198
    N199[primaryExp]
    N198 --> N199
    N200[lVal]
    N199 --> N200
    N201(("_2"))
    N200 --> N201
    N202(("+"))
    N195 --> N202
    N203[mulExp]
    N195 --> N203
    N204[unaryExp]
    N203 --> N204
    N205[primaryExp]
    N204 --> N205
    N206[number]
    N205 --> N206
    N207(("2"))
    N206 --> N207
    N208((")"))
    N183 --> N208
    N209((">"))
    N178 --> N209
    N210[addExp]
    N178 --> N210
    N211[mulExp]
    N210 --> N211
    N212[unaryExp]
    N211 --> N212
    N213[primaryExp]
    N212 --> N213
    N214[number]
    N213 --> N214
    N215(("0"))
    N214 --> N215
    N216((")"))
    N171 --> N216
    N217((")"))
    N119 --> N217
    N218(("||"))
    N111 --> N218
    N219[lAndExp]
    N111 --> N219
    N220[eqExp]
    N219 --> N220
    N221[relExp]
    N220 --> N221
    N222[addExp]
    N221 --> N222
    N223[mulExp]
    N222 --> N223
    N224[unaryExp]
    N223 --> N224
    N225[primaryExp]
    N224 --> N225
    N226(("("))
    N225 --> N226
    N227[exp]
    N225 --> N227
    N228[assignExp]
    N227 --> N228
    N229[lOrExp]
    N228 --> N229
    N230[lAndExp]
    N229 --> N230
    N231[eqExp]
    N230 --> N231
    N232[relExp]
    N231 --> N232
    N233[relExp]
    N232 --> N233
    N234[addExp]
    N233 --> N234
    N235[mulExp]
    N234 --> N235
    N236[unaryExp]
    N235 --> N236
    N237[primaryExp]
    N236 --> N237
    N238(("("))
    N237 --> N238
    N239[exp]
    N237 --> N239
    N240[assignExp]
    N239 --> N240
    N241[lVal]
    N240 --> N241
    N242(("_c"))
    N241 --> N242
    N243(("="))
    N240 --> N243
    N244[assignExp]
    N240 --> N244
    N245[lOrExp]
    N244 --> N245
    N246[lAndExp]
    N245 --> N246
    N247[eqExp]
    N246 --> N247
    N248[relExp]
    N247 --> N248
    N249[addExp]
    N248 --> N249
    N250[addExp]
    N249 --> N250
    N251[mulExp]
    N250 --> N251
    N252[unaryExp]
    N251 --> N252
    N253[primaryExp]
    N252 --> N253
    N254[lVal]
    N253 --> N254
    N255(("_3"))
    N254 --> N255
    N256(("+"))
    N249 --> N256
    N257[mulExp]
    N249 --> N257
    N258[unaryExp]
    N257 --> N258
    N259[primaryExp]
    N258 --> N259
    N260[number]
    N259 --> N260
    N261(("3"))
    N260 --> N261
    N262((")"))
    N237 --> N262
    N263((">"))
    N232 --> N263
    N264[addExp]
    N232 --> N264
    N265[mulExp]
    N264 --> N265
    N266[unaryExp]
    N265 --> N266
    N267[primaryExp]
    N266 --> N267
    N268[number]
    N267 --> N268
    N269(("0"))
    N268 --> N269
    N270((")"))
    N225 --> N270
    N271((")"))
    N107 --> N271
    N272[stmt]
    N107 --> N272
    N273[block]
    N272 --> N273
    N274(("{"))
    N273 --> N274
    N275[blockItem_list]
    N273 --> N275
    N276[blockItem_list]
    N275 --> N276
    N277[blockItem]
    N275 --> N277
    N278[stmt]
    N277 --> N278
    N279(("if"))
    N278 --> N279
    N280(("("))
    N278 --> N280
    N281[cond]
    N278 --> N281
    N282[lOrExp]
    N281 --> N282
    N283[lAndExp]
    N282 --> N283
    N284[eqExp]
    N283 --> N284
    N285[eqExp]
    N284 --> N285
    N286[relExp]
    N285 --> N286
    N287[addExp]
    N286 --> N287
    N288[mulExp]
    N287 --> N288
    N289[mulExp]
    N288 --> N289
    N290[unaryExp]
    N289 --> N290
    N291[primaryExp]
    N290 --> N291
    N292(("("))
    N291 --> N292
    N293[exp]
    N291 --> N293
    N294[assignExp]
    N293 --> N294
    N295[lOrExp]
    N294 --> N295
    N296[lAndExp]
    N295 --> N296
    N297[eqExp]
    N296 --> N297
    N298[relExp]
    N297 --> N298
    N299[addExp]
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
    N306(("_a"))
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
    N312(("_b"))
    N311 --> N312
    N313(("+"))
    N299 --> N313
    N314[mulExp]
    N299 --> N314
    N315[unaryExp]
    N314 --> N315
    N316[primaryExp]
    N315 --> N316
    N317[lVal]
    N316 --> N317
    N318(("_c"))
    N317 --> N318
    N319((")"))
    N291 --> N319
    N320(("%"))
    N288 --> N320
    N321[unaryExp]
    N288 --> N321
    N322[primaryExp]
    N321 --> N322
    N323[number]
    N322 --> N323
    N324(("2"))
    N323 --> N324
    N325(("=="))
    N284 --> N325
    N326[relExp]
    N284 --> N326
    N327[addExp]
    N326 --> N327
    N328[mulExp]
    N327 --> N328
    N329[unaryExp]
    N328 --> N329
    N330[primaryExp]
    N329 --> N330
    N331[number]
    N330 --> N331
    N332(("0"))
    N331 --> N332
    N333((")"))
    N278 --> N333
    N334[stmt]
    N278 --> N334
    N335[block]
    N334 --> N335
    N336(("{"))
    N335 --> N336
    N337[blockItem_list]
    N335 --> N337
    N338[blockItem_list]
    N337 --> N338
    N339[blockItem_list]
    N338 --> N339
    N340[blockItem]
    N338 --> N340
    N341[stmt]
    N340 --> N341
    N342[exp_opt]
    N341 --> N342
    N343[exp]
    N342 --> N343
    N344[assignExp]
    N343 --> N344
    N345[lOrExp]
    N344 --> N345
    N346[lAndExp]
    N345 --> N346
    N347[eqExp]
    N346 --> N347
    N348[relExp]
    N347 --> N348
    N349[addExp]
    N348 --> N349
    N350[addExp]
    N349 --> N350
    N351[addExp]
    N350 --> N351
    N352[mulExp]
    N351 --> N352
    N353[unaryExp]
    N352 --> N353
    N354[primaryExp]
    N353 --> N354
    N355[lVal]
    N354 --> N355
    N356(("_a"))
    N355 --> N356
    N357(("+"))
    N350 --> N357
    N358[mulExp]
    N350 --> N358
    N359[unaryExp]
    N358 --> N359
    N360[primaryExp]
    N359 --> N360
    N361[lVal]
    N360 --> N361
    N362(("_b"))
    N361 --> N362
    N363(("+"))
    N349 --> N363
    N364[mulExp]
    N349 --> N364
    N365[unaryExp]
    N364 --> N365
    N366[primaryExp]
    N365 --> N366
    N367[lVal]
    N366 --> N367
    N368(("_c"))
    N367 --> N368
    N369((";"))
    N341 --> N369
    N370[blockItem]
    N337 --> N370
    N371[stmt]
    N370 --> N371
    N372(("return"))
    N371 --> N372
    N373[exp_opt]
    N371 --> N373
    N374((";"))
    N371 --> N374
    N375(("}"))
    N335 --> N375
    N376[else_opt]
    N278 --> N376
    N377(("else"))
    N376 --> N377
    N378[stmt]
    N376 --> N378
    N379[block]
    N378 --> N379
    N380(("{"))
    N379 --> N380
    N381[blockItem_list]
    N379 --> N381
    N382[blockItem_list]
    N381 --> N382
    N383[blockItem_list]
    N382 --> N383
    N384[blockItem]
    N382 --> N384
    N385[stmt]
    N384 --> N385
    N386[exp_opt]
    N385 --> N386
    N387[exp]
    N386 --> N387
    N388[assignExp]
    N387 --> N388
    N389[lOrExp]
    N388 --> N389
    N390[lAndExp]
    N389 --> N390
    N391[eqExp]
    N390 --> N391
    N392[relExp]
    N391 --> N392
    N393[addExp]
    N392 --> N393
    N394[addExp]
    N393 --> N394
    N395[addExp]
    N394 --> N395
    N396[mulExp]
    N395 --> N396
    N397[unaryExp]
    N396 --> N397
    N398[primaryExp]
    N397 --> N398
    N399[lVal]
    N398 --> N399
    N400(("_a"))
    N399 --> N400
    N401(("-"))
    N394 --> N401
    N402[mulExp]
    N394 --> N402
    N403[unaryExp]
    N402 --> N403
    N404[primaryExp]
    N403 --> N404
    N405[lVal]
    N404 --> N405
    N406(("_b"))
    N405 --> N406
    N407(("+"))
    N393 --> N407
    N408[mulExp]
    N393 --> N408
    N409[unaryExp]
    N408 --> N409
    N410[primaryExp]
    N409 --> N410
    N411[lVal]
    N410 --> N411
    N412(("_c"))
    N411 --> N412
    N413((";"))
    N385 --> N413
    N414[blockItem]
    N381 --> N414
    N415[stmt]
    N414 --> N415
    N416(("return"))
    N415 --> N416
    N417[exp_opt]
    N415 --> N417
    N418((";"))
    N415 --> N418
    N419(("}"))
    N379 --> N419
    N420(("}"))
    N273 --> N420
    N421[else_opt]
    N107 --> N421
    N422[blockItem]
    N31 --> N422
    N423[stmt]
    N422 --> N423
    N424(("if"))
    N423 --> N424
    N425(("("))
    N423 --> N425
    N426[cond]
    N423 --> N426
    N427[lOrExp]
    N426 --> N427
    N428[lAndExp]
    N427 --> N428
    N429[eqExp]
    N428 --> N429
    N430[relExp]
    N429 --> N430
    N431[addExp]
    N430 --> N431
    N432[mulExp]
    N431 --> N432
    N433[unaryExp]
    N432 --> N433
    N434[primaryExp]
    N433 --> N434
    N435(("("))
    N434 --> N435
    N436[exp]
    N434 --> N436
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
    N446(("("))
    N445 --> N446
    N447[exp]
    N445 --> N447
    N448[assignExp]
    N447 --> N448
    N449[lOrExp]
    N448 --> N449
    N450[lAndExp]
    N449 --> N450
    N451[eqExp]
    N450 --> N451
    N452[relExp]
    N451 --> N452
    N453[addExp]
    N452 --> N453
    N454[mulExp]
    N453 --> N454
    N455[unaryExp]
    N454 --> N455
    N456[primaryExp]
    N455 --> N456
    N457(("("))
    N456 --> N457
    N458[exp]
    N456 --> N458
    N459[assignExp]
    N458 --> N459
    N460[lOrExp]
    N459 --> N460
    N461[lAndExp]
    N460 --> N461
    N462[eqExp]
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
    N469[lVal]
    N468 --> N469
    N470(("_a"))
    N469 --> N470
    N471(("=="))
    N462 --> N471
    N472[relExp]
    N462 --> N472
    N473[addExp]
    N472 --> N473
    N474[mulExp]
    N473 --> N474
    N475[unaryExp]
    N474 --> N475
    N476[primaryExp]
    N475 --> N476
    N477[number]
    N476 --> N477
    N478(("1"))
    N477 --> N478
    N479((")"))
    N456 --> N479
    N480((")"))
    N445 --> N480
    N481((")"))
    N434 --> N481
    N482((")"))
    N423 --> N482
    N483[stmt]
    N423 --> N483
    N484[block]
    N483 --> N484
    N485(("{"))
    N484 --> N485
    N486[blockItem_list]
    N484 --> N486
    N487[blockItem_list]
    N486 --> N487
    N488[blockItem_list]
    N487 --> N488
    N489[blockItem]
    N487 --> N489
    N490[stmt]
    N489 --> N490
    N491[exp_opt]
    N490 --> N491
    N492[exp]
    N491 --> N492
    N493[assignExp]
    N492 --> N493
    N494[lOrExp]
    N493 --> N494
    N495[lAndExp]
    N494 --> N495
    N496[eqExp]
    N495 --> N496
    N497[relExp]
    N496 --> N497
    N498[addExp]
    N497 --> N498
    N499[addExp]
    N498 --> N499
    N500[mulExp]
    N499 --> N500
    N501[unaryExp]
    N500 --> N501
    N502[primaryExp]
    N501 --> N502
    N503[lVal]
    N502 --> N503
    N504(("_b"))
    N503 --> N504
    N505(("+"))
    N498 --> N505
    N506[mulExp]
    N498 --> N506
    N507[unaryExp]
    N506 --> N507
    N508[primaryExp]
    N507 --> N508
    N509[lVal]
    N508 --> N509
    N510(("_c"))
    N509 --> N510
    N511((";"))
    N490 --> N511
    N512[blockItem]
    N486 --> N512
    N513[stmt]
    N512 --> N513
    N514(("return"))
    N513 --> N514
    N515[exp_opt]
    N513 --> N515
    N516((";"))
    N513 --> N516
    N517(("}"))
    N484 --> N517
    N518[else_opt]
    N423 --> N518
    N519(("else"))
    N518 --> N519
    N520[stmt]
    N518 --> N520
    N521[block]
    N520 --> N521
    N522(("{"))
    N521 --> N522
    N523[blockItem_list]
    N521 --> N523
    N524[blockItem_list]
    N523 --> N524
    N525[blockItem]
    N523 --> N525
    N526[stmt]
    N525 --> N526
    N527(("return"))
    N526 --> N527
    N528[exp_opt]
    N526 --> N528
    N529((";"))
    N526 --> N529
    N530(("}"))
    N521 --> N530
    N531(("}"))
    N29 --> N531
    N532(("}"))
    N1 --> N532
```
