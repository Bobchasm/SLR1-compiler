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
    N19[blockItem_list]
    N18 --> N19
    N20[blockItem_list]
    N19 --> N20
    N21[blockItem_list]
    N20 --> N21
    N22[blockItem_list]
    N21 --> N22
    N23[blockItem_list]
    N22 --> N23
    N24[blockItem_list]
    N23 --> N24
    N25[blockItem_list]
    N24 --> N25
    N26[blockItem_list]
    N25 --> N26
    N27[blockItem_list]
    N26 --> N27
    N28[blockItem_list]
    N27 --> N28
    N29[blockItem]
    N27 --> N29
    N30[decl]
    N29 --> N30
    N31[varDecl]
    N30 --> N31
    N32[bType]
    N31 --> N32
    N33(("int"))
    N32 --> N33
    N34[varDef]
    N31 --> N34
    N35(("a"))
    N34 --> N35
    N36(("="))
    N34 --> N36
    N37[initVal]
    N34 --> N37
    N38[exp]
    N37 --> N38
    N39[assignExp]
    N38 --> N39
    N40[lOrExp]
    N39 --> N40
    N41[lAndExp]
    N40 --> N41
    N42[eqExp]
    N41 --> N42
    N43[relExp]
    N42 --> N43
    N44[addExp]
    N43 --> N44
    N45[mulExp]
    N44 --> N45
    N46[unaryExp]
    N45 --> N46
    N47[primaryExp]
    N46 --> N47
    N48[number]
    N47 --> N48
    N49(("10"))
    N48 --> N49
    N50[varDef_list]
    N31 --> N50
    N51((";"))
    N31 --> N51
    N52[blockItem]
    N26 --> N52
    N53[decl]
    N52 --> N53
    N54[varDecl]
    N53 --> N54
    N55[bType]
    N54 --> N55
    N56(("int"))
    N55 --> N56
    N57[varDef]
    N54 --> N57
    N58(("b"))
    N57 --> N58
    N59(("="))
    N57 --> N59
    N60[initVal]
    N57 --> N60
    N61[exp]
    N60 --> N61
    N62[assignExp]
    N61 --> N62
    N63[lOrExp]
    N62 --> N63
    N64[lAndExp]
    N63 --> N64
    N65[eqExp]
    N64 --> N65
    N66[relExp]
    N65 --> N66
    N67[addExp]
    N66 --> N67
    N68[mulExp]
    N67 --> N68
    N69[unaryExp]
    N68 --> N69
    N70[primaryExp]
    N69 --> N70
    N71[number]
    N70 --> N71
    N72(("3"))
    N71 --> N72
    N73[varDef_list]
    N54 --> N73
    N74((";"))
    N54 --> N74
    N75[blockItem]
    N25 --> N75
    N76[decl]
    N75 --> N76
    N77[varDecl]
    N76 --> N77
    N78[bType]
    N77 --> N78
    N79(("int"))
    N78 --> N79
    N80[varDef]
    N77 --> N80
    N81(("result"))
    N80 --> N81
    N82[varDef_list]
    N77 --> N82
    N83((";"))
    N77 --> N83
    N84[blockItem]
    N24 --> N84
    N85[stmt]
    N84 --> N85
    N86[exp_opt]
    N85 --> N86
    N87[exp]
    N86 --> N87
    N88[assignExp]
    N87 --> N88
    N89[lVal]
    N88 --> N89
    N90(("result"))
    N89 --> N90
    N91(("="))
    N88 --> N91
    N92[assignExp]
    N88 --> N92
    N93[lOrExp]
    N92 --> N93
    N94[lAndExp]
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
    N110((";"))
    N85 --> N110
    N111[blockItem]
    N23 --> N111
    N112[stmt]
    N111 --> N112
    N113[exp_opt]
    N112 --> N113
    N114[exp]
    N113 --> N114
    N115[assignExp]
    N114 --> N115
    N116[lVal]
    N115 --> N116
    N117(("result"))
    N116 --> N117
    N118(("="))
    N115 --> N118
    N119[assignExp]
    N115 --> N119
    N120[lOrExp]
    N119 --> N120
    N121[lAndExp]
    N120 --> N121
    N122[eqExp]
    N121 --> N122
    N123[relExp]
    N122 --> N123
    N124[addExp]
    N123 --> N124
    N125[addExp]
    N124 --> N125
    N126[mulExp]
    N125 --> N126
    N127[unaryExp]
    N126 --> N127
    N128[primaryExp]
    N127 --> N128
    N129[lVal]
    N128 --> N129
    N130(("a"))
    N129 --> N130
    N131(("-"))
    N124 --> N131
    N132[mulExp]
    N124 --> N132
    N133[unaryExp]
    N132 --> N133
    N134[primaryExp]
    N133 --> N134
    N135[lVal]
    N134 --> N135
    N136(("b"))
    N135 --> N136
    N137((";"))
    N112 --> N137
    N138[blockItem]
    N22 --> N138
    N139[stmt]
    N138 --> N139
    N140[exp_opt]
    N139 --> N140
    N141[exp]
    N140 --> N141
    N142[assignExp]
    N141 --> N142
    N143[lVal]
    N142 --> N143
    N144(("result"))
    N143 --> N144
    N145(("="))
    N142 --> N145
    N146[assignExp]
    N142 --> N146
    N147[lOrExp]
    N146 --> N147
    N148[lAndExp]
    N147 --> N148
    N149[eqExp]
    N148 --> N149
    N150[relExp]
    N149 --> N150
    N151[addExp]
    N150 --> N151
    N152[mulExp]
    N151 --> N152
    N153[mulExp]
    N152 --> N153
    N154[unaryExp]
    N153 --> N154
    N155[primaryExp]
    N154 --> N155
    N156[lVal]
    N155 --> N156
    N157(("a"))
    N156 --> N157
    N158(("*"))
    N152 --> N158
    N159[unaryExp]
    N152 --> N159
    N160[primaryExp]
    N159 --> N160
    N161[lVal]
    N160 --> N161
    N162(("b"))
    N161 --> N162
    N163((";"))
    N139 --> N163
    N164[blockItem]
    N21 --> N164
    N165[stmt]
    N164 --> N165
    N166[exp_opt]
    N165 --> N166
    N167[exp]
    N166 --> N167
    N168[assignExp]
    N167 --> N168
    N169[lVal]
    N168 --> N169
    N170(("result"))
    N169 --> N170
    N171(("="))
    N168 --> N171
    N172[assignExp]
    N168 --> N172
    N173[lOrExp]
    N172 --> N173
    N174[lAndExp]
    N173 --> N174
    N175[eqExp]
    N174 --> N175
    N176[relExp]
    N175 --> N176
    N177[addExp]
    N176 --> N177
    N178[mulExp]
    N177 --> N178
    N179[mulExp]
    N178 --> N179
    N180[unaryExp]
    N179 --> N180
    N181[primaryExp]
    N180 --> N181
    N182[lVal]
    N181 --> N182
    N183(("a"))
    N182 --> N183
    N184(("/"))
    N178 --> N184
    N185[unaryExp]
    N178 --> N185
    N186[primaryExp]
    N185 --> N186
    N187[lVal]
    N186 --> N187
    N188(("b"))
    N187 --> N188
    N189((";"))
    N165 --> N189
    N190[blockItem]
    N20 --> N190
    N191[stmt]
    N190 --> N191
    N192[exp_opt]
    N191 --> N192
    N193[exp]
    N192 --> N193
    N194[assignExp]
    N193 --> N194
    N195[lVal]
    N194 --> N195
    N196(("result"))
    N195 --> N196
    N197(("="))
    N194 --> N197
    N198[assignExp]
    N194 --> N198
    N199[lOrExp]
    N198 --> N199
    N200[lAndExp]
    N199 --> N200
    N201[eqExp]
    N200 --> N201
    N202[relExp]
    N201 --> N202
    N203[addExp]
    N202 --> N203
    N204[mulExp]
    N203 --> N204
    N205[mulExp]
    N204 --> N205
    N206[unaryExp]
    N205 --> N206
    N207[primaryExp]
    N206 --> N207
    N208[lVal]
    N207 --> N208
    N209(("a"))
    N208 --> N209
    N210(("%"))
    N204 --> N210
    N211[unaryExp]
    N204 --> N211
    N212[primaryExp]
    N211 --> N212
    N213[lVal]
    N212 --> N213
    N214(("b"))
    N213 --> N214
    N215((";"))
    N191 --> N215
    N216[blockItem]
    N19 --> N216
    N217[stmt]
    N216 --> N217
    N218(("if"))
    N217 --> N218
    N219(("("))
    N217 --> N219
    N220[cond]
    N217 --> N220
    N221[lOrExp]
    N220 --> N221
    N222[lAndExp]
    N221 --> N222
    N223[eqExp]
    N222 --> N223
    N224[relExp]
    N223 --> N224
    N225[relExp]
    N224 --> N225
    N226[addExp]
    N225 --> N226
    N227[mulExp]
    N226 --> N227
    N228[unaryExp]
    N227 --> N228
    N229[primaryExp]
    N228 --> N229
    N230[lVal]
    N229 --> N230
    N231(("a"))
    N230 --> N231
    N232((">"))
    N224 --> N232
    N233[addExp]
    N224 --> N233
    N234[mulExp]
    N233 --> N234
    N235[unaryExp]
    N234 --> N235
    N236[primaryExp]
    N235 --> N236
    N237[lVal]
    N236 --> N237
    N238(("b"))
    N237 --> N238
    N239((")"))
    N217 --> N239
    N240[stmt]
    N217 --> N240
    N241[exp_opt]
    N240 --> N241
    N242[exp]
    N241 --> N242
    N243[assignExp]
    N242 --> N243
    N244[lVal]
    N243 --> N244
    N245(("result"))
    N244 --> N245
    N246(("="))
    N243 --> N246
    N247[assignExp]
    N243 --> N247
    N248[lOrExp]
    N247 --> N248
    N249[lAndExp]
    N248 --> N249
    N250[eqExp]
    N249 --> N250
    N251[relExp]
    N250 --> N251
    N252[addExp]
    N251 --> N252
    N253[mulExp]
    N252 --> N253
    N254[unaryExp]
    N253 --> N254
    N255[primaryExp]
    N254 --> N255
    N256[number]
    N255 --> N256
    N257(("1"))
    N256 --> N257
    N258((";"))
    N240 --> N258
    N259[else_opt]
    N217 --> N259
    N260[blockItem]
    N18 --> N260
    N261[stmt]
    N260 --> N261
    N262(("if"))
    N261 --> N262
    N263(("("))
    N261 --> N263
    N264[cond]
    N261 --> N264
    N265[lOrExp]
    N264 --> N265
    N266[lAndExp]
    N265 --> N266
    N267[eqExp]
    N266 --> N267
    N268[relExp]
    N267 --> N268
    N269[relExp]
    N268 --> N269
    N270[addExp]
    N269 --> N270
    N271[mulExp]
    N270 --> N271
    N272[unaryExp]
    N271 --> N272
    N273[primaryExp]
    N272 --> N273
    N274[lVal]
    N273 --> N274
    N275(("a"))
    N274 --> N275
    N276(("<"))
    N268 --> N276
    N277[addExp]
    N268 --> N277
    N278[mulExp]
    N277 --> N278
    N279[unaryExp]
    N278 --> N279
    N280[primaryExp]
    N279 --> N280
    N281[lVal]
    N280 --> N281
    N282(("b"))
    N281 --> N282
    N283((")"))
    N261 --> N283
    N284[stmt]
    N261 --> N284
    N285[exp_opt]
    N284 --> N285
    N286[exp]
    N285 --> N286
    N287[assignExp]
    N286 --> N287
    N288[lVal]
    N287 --> N288
    N289(("result"))
    N288 --> N289
    N290(("="))
    N287 --> N290
    N291[assignExp]
    N287 --> N291
    N292[lOrExp]
    N291 --> N292
    N293[lAndExp]
    N292 --> N293
    N294[eqExp]
    N293 --> N294
    N295[relExp]
    N294 --> N295
    N296[addExp]
    N295 --> N296
    N297[mulExp]
    N296 --> N297
    N298[unaryExp]
    N297 --> N298
    N299[primaryExp]
    N298 --> N299
    N300[number]
    N299 --> N300
    N301(("2"))
    N300 --> N301
    N302((";"))
    N284 --> N302
    N303[else_opt]
    N261 --> N303
    N304[blockItem]
    N17 --> N304
    N305[stmt]
    N304 --> N305
    N306(("if"))
    N305 --> N306
    N307(("("))
    N305 --> N307
    N308[cond]
    N305 --> N308
    N309[lOrExp]
    N308 --> N309
    N310[lAndExp]
    N309 --> N310
    N311[eqExp]
    N310 --> N311
    N312[relExp]
    N311 --> N312
    N313[relExp]
    N312 --> N313
    N314[addExp]
    N313 --> N314
    N315[mulExp]
    N314 --> N315
    N316[unaryExp]
    N315 --> N316
    N317[primaryExp]
    N316 --> N317
    N318[lVal]
    N317 --> N318
    N319(("a"))
    N318 --> N319
    N320((">="))
    N312 --> N320
    N321[addExp]
    N312 --> N321
    N322[mulExp]
    N321 --> N322
    N323[unaryExp]
    N322 --> N323
    N324[primaryExp]
    N323 --> N324
    N325[lVal]
    N324 --> N325
    N326(("b"))
    N325 --> N326
    N327((")"))
    N305 --> N327
    N328[stmt]
    N305 --> N328
    N329[exp_opt]
    N328 --> N329
    N330[exp]
    N329 --> N330
    N331[assignExp]
    N330 --> N331
    N332[lVal]
    N331 --> N332
    N333(("result"))
    N332 --> N333
    N334(("="))
    N331 --> N334
    N335[assignExp]
    N331 --> N335
    N336[lOrExp]
    N335 --> N336
    N337[lAndExp]
    N336 --> N337
    N338[eqExp]
    N337 --> N338
    N339[relExp]
    N338 --> N339
    N340[addExp]
    N339 --> N340
    N341[mulExp]
    N340 --> N341
    N342[unaryExp]
    N341 --> N342
    N343[primaryExp]
    N342 --> N343
    N344[number]
    N343 --> N344
    N345(("3"))
    N344 --> N345
    N346((";"))
    N328 --> N346
    N347[else_opt]
    N305 --> N347
    N348[blockItem]
    N16 --> N348
    N349[stmt]
    N348 --> N349
    N350(("if"))
    N349 --> N350
    N351(("("))
    N349 --> N351
    N352[cond]
    N349 --> N352
    N353[lOrExp]
    N352 --> N353
    N354[lAndExp]
    N353 --> N354
    N355[eqExp]
    N354 --> N355
    N356[relExp]
    N355 --> N356
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
    N362[lVal]
    N361 --> N362
    N363(("a"))
    N362 --> N363
    N364(("<="))
    N356 --> N364
    N365[addExp]
    N356 --> N365
    N366[mulExp]
    N365 --> N366
    N367[unaryExp]
    N366 --> N367
    N368[primaryExp]
    N367 --> N368
    N369[lVal]
    N368 --> N369
    N370(("b"))
    N369 --> N370
    N371((")"))
    N349 --> N371
    N372[stmt]
    N349 --> N372
    N373[exp_opt]
    N372 --> N373
    N374[exp]
    N373 --> N374
    N375[assignExp]
    N374 --> N375
    N376[lVal]
    N375 --> N376
    N377(("result"))
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
    N389(("4"))
    N388 --> N389
    N390((";"))
    N372 --> N390
    N391[else_opt]
    N349 --> N391
    N392[blockItem]
    N15 --> N392
    N393[stmt]
    N392 --> N393
    N394(("if"))
    N393 --> N394
    N395(("("))
    N393 --> N395
    N396[cond]
    N393 --> N396
    N397[lOrExp]
    N396 --> N397
    N398[lAndExp]
    N397 --> N398
    N399[eqExp]
    N398 --> N399
    N400[eqExp]
    N399 --> N400
    N401[relExp]
    N400 --> N401
    N402[addExp]
    N401 --> N402
    N403[mulExp]
    N402 --> N403
    N404[unaryExp]
    N403 --> N404
    N405[primaryExp]
    N404 --> N405
    N406[lVal]
    N405 --> N406
    N407(("a"))
    N406 --> N407
    N408(("=="))
    N399 --> N408
    N409[relExp]
    N399 --> N409
    N410[addExp]
    N409 --> N410
    N411[mulExp]
    N410 --> N411
    N412[unaryExp]
    N411 --> N412
    N413[primaryExp]
    N412 --> N413
    N414[lVal]
    N413 --> N414
    N415(("b"))
    N414 --> N415
    N416((")"))
    N393 --> N416
    N417[stmt]
    N393 --> N417
    N418[exp_opt]
    N417 --> N418
    N419[exp]
    N418 --> N419
    N420[assignExp]
    N419 --> N420
    N421[lVal]
    N420 --> N421
    N422(("result"))
    N421 --> N422
    N423(("="))
    N420 --> N423
    N424[assignExp]
    N420 --> N424
    N425[lOrExp]
    N424 --> N425
    N426[lAndExp]
    N425 --> N426
    N427[eqExp]
    N426 --> N427
    N428[relExp]
    N427 --> N428
    N429[addExp]
    N428 --> N429
    N430[mulExp]
    N429 --> N430
    N431[unaryExp]
    N430 --> N431
    N432[primaryExp]
    N431 --> N432
    N433[number]
    N432 --> N433
    N434(("5"))
    N433 --> N434
    N435((";"))
    N417 --> N435
    N436[else_opt]
    N393 --> N436
    N437[blockItem]
    N14 --> N437
    N438[stmt]
    N437 --> N438
    N439(("if"))
    N438 --> N439
    N440(("("))
    N438 --> N440
    N441[cond]
    N438 --> N441
    N442[lOrExp]
    N441 --> N442
    N443[lAndExp]
    N442 --> N443
    N444[eqExp]
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
    N450[primaryExp]
    N449 --> N450
    N451[lVal]
    N450 --> N451
    N452(("a"))
    N451 --> N452
    N453(("!="))
    N444 --> N453
    N454[relExp]
    N444 --> N454
    N455[addExp]
    N454 --> N455
    N456[mulExp]
    N455 --> N456
    N457[unaryExp]
    N456 --> N457
    N458[primaryExp]
    N457 --> N458
    N459[lVal]
    N458 --> N459
    N460(("b"))
    N459 --> N460
    N461((")"))
    N438 --> N461
    N462[stmt]
    N438 --> N462
    N463[exp_opt]
    N462 --> N463
    N464[exp]
    N463 --> N464
    N465[assignExp]
    N464 --> N465
    N466[lVal]
    N465 --> N466
    N467(("result"))
    N466 --> N467
    N468(("="))
    N465 --> N468
    N469[assignExp]
    N465 --> N469
    N470[lOrExp]
    N469 --> N470
    N471[lAndExp]
    N470 --> N471
    N472[eqExp]
    N471 --> N472
    N473[relExp]
    N472 --> N473
    N474[addExp]
    N473 --> N474
    N475[mulExp]
    N474 --> N475
    N476[unaryExp]
    N475 --> N476
    N477[primaryExp]
    N476 --> N477
    N478[number]
    N477 --> N478
    N479(("6"))
    N478 --> N479
    N480((";"))
    N462 --> N480
    N481[else_opt]
    N438 --> N481
    N482[blockItem]
    N13 --> N482
    N483[stmt]
    N482 --> N483
    N484(("return"))
    N483 --> N484
    N485[exp_opt]
    N483 --> N485
    N486[exp]
    N485 --> N486
    N487[assignExp]
    N486 --> N487
    N488[lOrExp]
    N487 --> N488
    N489[lAndExp]
    N488 --> N489
    N490[eqExp]
    N489 --> N490
    N491[relExp]
    N490 --> N491
    N492[addExp]
    N491 --> N492
    N493[mulExp]
    N492 --> N493
    N494[unaryExp]
    N493 --> N494
    N495[primaryExp]
    N494 --> N495
    N496[lVal]
    N495 --> N496
    N497(("result"))
    N496 --> N497
    N498((";"))
    N483 --> N498
    N499(("}"))
    N11 --> N499
    N500(("}"))
    N1 --> N500
```
