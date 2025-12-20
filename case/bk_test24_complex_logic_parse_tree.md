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
    N25[blockItem]
    N23 --> N25
    N26[decl]
    N25 --> N26
    N27[varDecl]
    N26 --> N27
    N28[bType]
    N27 --> N28
    N29(("int"))
    N28 --> N29
    N30[varDef]
    N27 --> N30
    N31(("a"))
    N30 --> N31
    N32(("="))
    N30 --> N32
    N33[initVal]
    N30 --> N33
    N34[exp]
    N33 --> N34
    N35[assignExp]
    N34 --> N35
    N36[lOrExp]
    N35 --> N36
    N37[lAndExp]
    N36 --> N37
    N38[eqExp]
    N37 --> N38
    N39[relExp]
    N38 --> N39
    N40[addExp]
    N39 --> N40
    N41[mulExp]
    N40 --> N41
    N42[unaryExp]
    N41 --> N42
    N43[primaryExp]
    N42 --> N43
    N44[number]
    N43 --> N44
    N45(("1"))
    N44 --> N45
    N46[varDef_list]
    N27 --> N46
    N47((";"))
    N27 --> N47
    N48[blockItem]
    N22 --> N48
    N49[decl]
    N48 --> N49
    N50[varDecl]
    N49 --> N50
    N51[bType]
    N50 --> N51
    N52(("int"))
    N51 --> N52
    N53[varDef]
    N50 --> N53
    N54(("b"))
    N53 --> N54
    N55(("="))
    N53 --> N55
    N56[initVal]
    N53 --> N56
    N57[exp]
    N56 --> N57
    N58[assignExp]
    N57 --> N58
    N59[lOrExp]
    N58 --> N59
    N60[lAndExp]
    N59 --> N60
    N61[eqExp]
    N60 --> N61
    N62[relExp]
    N61 --> N62
    N63[addExp]
    N62 --> N63
    N64[mulExp]
    N63 --> N64
    N65[unaryExp]
    N64 --> N65
    N66[primaryExp]
    N65 --> N66
    N67[number]
    N66 --> N67
    N68(("0"))
    N67 --> N68
    N69[varDef_list]
    N50 --> N69
    N70((";"))
    N50 --> N70
    N71[blockItem]
    N21 --> N71
    N72[decl]
    N71 --> N72
    N73[varDecl]
    N72 --> N73
    N74[bType]
    N73 --> N74
    N75(("int"))
    N74 --> N75
    N76[varDef]
    N73 --> N76
    N77(("c"))
    N76 --> N77
    N78(("="))
    N76 --> N78
    N79[initVal]
    N76 --> N79
    N80[exp]
    N79 --> N80
    N81[assignExp]
    N80 --> N81
    N82[lOrExp]
    N81 --> N82
    N83[lAndExp]
    N82 --> N83
    N84[eqExp]
    N83 --> N84
    N85[relExp]
    N84 --> N85
    N86[addExp]
    N85 --> N86
    N87[mulExp]
    N86 --> N87
    N88[unaryExp]
    N87 --> N88
    N89[primaryExp]
    N88 --> N89
    N90[number]
    N89 --> N90
    N91(("2"))
    N90 --> N91
    N92[varDef_list]
    N73 --> N92
    N93((";"))
    N73 --> N93
    N94[blockItem]
    N20 --> N94
    N95[decl]
    N94 --> N95
    N96[varDecl]
    N95 --> N96
    N97[bType]
    N96 --> N97
    N98(("int"))
    N97 --> N98
    N99[varDef]
    N96 --> N99
    N100(("result1"))
    N99 --> N100
    N101[varDef_list]
    N96 --> N101
    N102((";"))
    N96 --> N102
    N103[blockItem]
    N19 --> N103
    N104[stmt]
    N103 --> N104
    N105(("if"))
    N104 --> N105
    N106(("("))
    N104 --> N106
    N107[cond]
    N104 --> N107
    N108[lOrExp]
    N107 --> N108
    N109[lOrExp]
    N108 --> N109
    N110[lAndExp]
    N109 --> N110
    N111[eqExp]
    N110 --> N111
    N112[relExp]
    N111 --> N112
    N113[addExp]
    N112 --> N113
    N114[mulExp]
    N113 --> N114
    N115[unaryExp]
    N114 --> N115
    N116[primaryExp]
    N115 --> N116
    N117(("("))
    N116 --> N117
    N118[exp]
    N116 --> N118
    N119[assignExp]
    N118 --> N119
    N120[lOrExp]
    N119 --> N120
    N121[lAndExp]
    N120 --> N121
    N122[lAndExp]
    N121 --> N122
    N123[eqExp]
    N122 --> N123
    N124[relExp]
    N123 --> N124
    N125[relExp]
    N124 --> N125
    N126[addExp]
    N125 --> N126
    N127[mulExp]
    N126 --> N127
    N128[unaryExp]
    N127 --> N128
    N129[primaryExp]
    N128 --> N129
    N130[lVal]
    N129 --> N130
    N131(("a"))
    N130 --> N131
    N132((">"))
    N124 --> N132
    N133[addExp]
    N124 --> N133
    N134[mulExp]
    N133 --> N134
    N135[unaryExp]
    N134 --> N135
    N136[primaryExp]
    N135 --> N136
    N137[number]
    N136 --> N137
    N138(("0"))
    N137 --> N138
    N139(("&&"))
    N121 --> N139
    N140[eqExp]
    N121 --> N140
    N141[relExp]
    N140 --> N141
    N142[relExp]
    N141 --> N142
    N143[addExp]
    N142 --> N143
    N144[mulExp]
    N143 --> N144
    N145[unaryExp]
    N144 --> N145
    N146[primaryExp]
    N145 --> N146
    N147[lVal]
    N146 --> N147
    N148(("b"))
    N147 --> N148
    N149((">="))
    N141 --> N149
    N150[addExp]
    N141 --> N150
    N151[mulExp]
    N150 --> N151
    N152[unaryExp]
    N151 --> N152
    N153[primaryExp]
    N152 --> N153
    N154[number]
    N153 --> N154
    N155(("0"))
    N154 --> N155
    N156((")"))
    N116 --> N156
    N157(("||"))
    N108 --> N157
    N158[lAndExp]
    N108 --> N158
    N159[eqExp]
    N158 --> N159
    N160[relExp]
    N159 --> N160
    N161[addExp]
    N160 --> N161
    N162[mulExp]
    N161 --> N162
    N163[unaryExp]
    N162 --> N163
    N164[primaryExp]
    N163 --> N164
    N165(("("))
    N164 --> N165
    N166[exp]
    N164 --> N166
    N167[assignExp]
    N166 --> N167
    N168[lOrExp]
    N167 --> N168
    N169[lAndExp]
    N168 --> N169
    N170[lAndExp]
    N169 --> N170
    N171[eqExp]
    N170 --> N171
    N172[relExp]
    N171 --> N172
    N173[relExp]
    N172 --> N173
    N174[addExp]
    N173 --> N174
    N175[mulExp]
    N174 --> N175
    N176[unaryExp]
    N175 --> N176
    N177[primaryExp]
    N176 --> N177
    N178[lVal]
    N177 --> N178
    N179(("c"))
    N178 --> N179
    N180(("<="))
    N172 --> N180
    N181[addExp]
    N172 --> N181
    N182[mulExp]
    N181 --> N182
    N183[unaryExp]
    N182 --> N183
    N184[primaryExp]
    N183 --> N184
    N185[number]
    N184 --> N185
    N186(("2"))
    N185 --> N186
    N187(("&&"))
    N169 --> N187
    N188[eqExp]
    N169 --> N188
    N189[eqExp]
    N188 --> N189
    N190[relExp]
    N189 --> N190
    N191[addExp]
    N190 --> N191
    N192[mulExp]
    N191 --> N192
    N193[unaryExp]
    N192 --> N193
    N194[primaryExp]
    N193 --> N194
    N195[lVal]
    N194 --> N195
    N196(("a"))
    N195 --> N196
    N197(("!="))
    N188 --> N197
    N198[relExp]
    N188 --> N198
    N199[addExp]
    N198 --> N199
    N200[mulExp]
    N199 --> N200
    N201[unaryExp]
    N200 --> N201
    N202[primaryExp]
    N201 --> N202
    N203[lVal]
    N202 --> N203
    N204(("b"))
    N203 --> N204
    N205((")"))
    N164 --> N205
    N206((")"))
    N104 --> N206
    N207[stmt]
    N104 --> N207
    N208[block]
    N207 --> N208
    N209(("{"))
    N208 --> N209
    N210[blockItem_list]
    N208 --> N210
    N211[blockItem_list]
    N210 --> N211
    N212[blockItem]
    N210 --> N212
    N213[stmt]
    N212 --> N213
    N214[exp_opt]
    N213 --> N214
    N215[exp]
    N214 --> N215
    N216[assignExp]
    N215 --> N216
    N217[lVal]
    N216 --> N217
    N218(("result1"))
    N217 --> N218
    N219(("="))
    N216 --> N219
    N220[assignExp]
    N216 --> N220
    N221[lOrExp]
    N220 --> N221
    N222[lAndExp]
    N221 --> N222
    N223[eqExp]
    N222 --> N223
    N224[relExp]
    N223 --> N224
    N225[addExp]
    N224 --> N225
    N226[mulExp]
    N225 --> N226
    N227[unaryExp]
    N226 --> N227
    N228[primaryExp]
    N227 --> N228
    N229[number]
    N228 --> N229
    N230(("10"))
    N229 --> N230
    N231((";"))
    N213 --> N231
    N232(("}"))
    N208 --> N232
    N233[else_opt]
    N104 --> N233
    N234[blockItem]
    N18 --> N234
    N235[stmt]
    N234 --> N235
    N236(("if"))
    N235 --> N236
    N237(("("))
    N235 --> N237
    N238[cond]
    N235 --> N238
    N239[lOrExp]
    N238 --> N239
    N240[lOrExp]
    N239 --> N240
    N241[lAndExp]
    N240 --> N241
    N242[eqExp]
    N241 --> N242
    N243[relExp]
    N242 --> N243
    N244[addExp]
    N243 --> N244
    N245[mulExp]
    N244 --> N245
    N246[unaryExp]
    N245 --> N246
    N247[primaryExp]
    N246 --> N247
    N248(("("))
    N247 --> N248
    N249[exp]
    N247 --> N249
    N250[assignExp]
    N249 --> N250
    N251[lOrExp]
    N250 --> N251
    N252[lAndExp]
    N251 --> N252
    N253[lAndExp]
    N252 --> N253
    N254[eqExp]
    N253 --> N254
    N255[relExp]
    N254 --> N255
    N256[addExp]
    N255 --> N256
    N257[mulExp]
    N256 --> N257
    N258[unaryExp]
    N257 --> N258
    N259[primaryExp]
    N258 --> N259
    N260[lVal]
    N259 --> N260
    N261(("a"))
    N260 --> N261
    N262(("&&"))
    N252 --> N262
    N263[eqExp]
    N252 --> N263
    N264[relExp]
    N263 --> N264
    N265[addExp]
    N264 --> N265
    N266[mulExp]
    N265 --> N266
    N267[unaryExp]
    N266 --> N267
    N268[primaryExp]
    N267 --> N268
    N269[lVal]
    N268 --> N269
    N270(("b"))
    N269 --> N270
    N271((")"))
    N247 --> N271
    N272(("||"))
    N239 --> N272
    N273[lAndExp]
    N239 --> N273
    N274[eqExp]
    N273 --> N274
    N275[relExp]
    N274 --> N275
    N276[addExp]
    N275 --> N276
    N277[mulExp]
    N276 --> N277
    N278[unaryExp]
    N277 --> N278
    N279[primaryExp]
    N278 --> N279
    N280(("("))
    N279 --> N280
    N281[exp]
    N279 --> N281
    N282[assignExp]
    N281 --> N282
    N283[lOrExp]
    N282 --> N283
    N284[lAndExp]
    N283 --> N284
    N285[eqExp]
    N284 --> N285
    N286[relExp]
    N285 --> N286
    N287[relExp]
    N286 --> N287
    N288[addExp]
    N287 --> N288
    N289[mulExp]
    N288 --> N289
    N290[unaryExp]
    N289 --> N290
    N291[primaryExp]
    N290 --> N291
    N292[lVal]
    N291 --> N292
    N293(("c"))
    N292 --> N293
    N294((">"))
    N286 --> N294
    N295[addExp]
    N286 --> N295
    N296[mulExp]
    N295 --> N296
    N297[unaryExp]
    N296 --> N297
    N298[primaryExp]
    N297 --> N298
    N299[number]
    N298 --> N299
    N300(("0"))
    N299 --> N300
    N301((")"))
    N279 --> N301
    N302((")"))
    N235 --> N302
    N303[stmt]
    N235 --> N303
    N304[block]
    N303 --> N304
    N305(("{"))
    N304 --> N305
    N306[blockItem_list]
    N304 --> N306
    N307[blockItem_list]
    N306 --> N307
    N308[blockItem]
    N306 --> N308
    N309[stmt]
    N308 --> N309
    N310(("return"))
    N309 --> N310
    N311[exp_opt]
    N309 --> N311
    N312[exp]
    N311 --> N312
    N313[assignExp]
    N312 --> N313
    N314[lOrExp]
    N313 --> N314
    N315[lAndExp]
    N314 --> N315
    N316[eqExp]
    N315 --> N316
    N317[relExp]
    N316 --> N317
    N318[addExp]
    N317 --> N318
    N319[mulExp]
    N318 --> N319
    N320[unaryExp]
    N319 --> N320
    N321[primaryExp]
    N320 --> N321
    N322[number]
    N321 --> N322
    N323(("1"))
    N322 --> N323
    N324((";"))
    N309 --> N324
    N325(("}"))
    N304 --> N325
    N326[else_opt]
    N235 --> N326
    N327(("else"))
    N326 --> N327
    N328[stmt]
    N326 --> N328
    N329[block]
    N328 --> N329
    N330(("{"))
    N329 --> N330
    N331[blockItem_list]
    N329 --> N331
    N332[blockItem_list]
    N331 --> N332
    N333[blockItem]
    N331 --> N333
    N334[stmt]
    N333 --> N334
    N335(("return"))
    N334 --> N335
    N336[exp_opt]
    N334 --> N336
    N337[exp]
    N336 --> N337
    N338[assignExp]
    N337 --> N338
    N339[lOrExp]
    N338 --> N339
    N340[lAndExp]
    N339 --> N340
    N341[eqExp]
    N340 --> N341
    N342[relExp]
    N341 --> N342
    N343[addExp]
    N342 --> N343
    N344[mulExp]
    N343 --> N344
    N345[unaryExp]
    N344 --> N345
    N346[primaryExp]
    N345 --> N346
    N347[number]
    N346 --> N347
    N348(("0"))
    N347 --> N348
    N349((";"))
    N334 --> N349
    N350(("}"))
    N329 --> N350
    N351[blockItem]
    N17 --> N351
    N352[decl]
    N351 --> N352
    N353[varDecl]
    N352 --> N353
    N354[bType]
    N353 --> N354
    N355(("int"))
    N354 --> N355
    N356[varDef]
    N353 --> N356
    N357(("x"))
    N356 --> N357
    N358(("="))
    N356 --> N358
    N359[initVal]
    N356 --> N359
    N360[exp]
    N359 --> N360
    N361[assignExp]
    N360 --> N361
    N362[lOrExp]
    N361 --> N362
    N363[lAndExp]
    N362 --> N363
    N364[eqExp]
    N363 --> N364
    N365[relExp]
    N364 --> N365
    N366[addExp]
    N365 --> N366
    N367[mulExp]
    N366 --> N367
    N368[unaryExp]
    N367 --> N368
    N369[primaryExp]
    N368 --> N369
    N370[number]
    N369 --> N370
    N371(("5"))
    N370 --> N371
    N372[varDef_list]
    N353 --> N372
    N373((";"))
    N353 --> N373
    N374[blockItem]
    N16 --> N374
    N375[decl]
    N374 --> N375
    N376[varDecl]
    N375 --> N376
    N377[bType]
    N376 --> N377
    N378(("int"))
    N377 --> N378
    N379[varDef]
    N376 --> N379
    N380(("y"))
    N379 --> N380
    N381(("="))
    N379 --> N381
    N382[initVal]
    N379 --> N382
    N383[exp]
    N382 --> N383
    N384[assignExp]
    N383 --> N384
    N385[lOrExp]
    N384 --> N385
    N386[lAndExp]
    N385 --> N386
    N387[eqExp]
    N386 --> N387
    N388[relExp]
    N387 --> N388
    N389[addExp]
    N388 --> N389
    N390[mulExp]
    N389 --> N390
    N391[unaryExp]
    N390 --> N391
    N392[primaryExp]
    N391 --> N392
    N393[number]
    N392 --> N393
    N394(("10"))
    N393 --> N394
    N395[varDef_list]
    N376 --> N395
    N396((";"))
    N376 --> N396
    N397[blockItem]
    N15 --> N397
    N398[decl]
    N397 --> N398
    N399[varDecl]
    N398 --> N399
    N400[bType]
    N399 --> N400
    N401(("int"))
    N400 --> N401
    N402[varDef]
    N399 --> N402
    N403(("z"))
    N402 --> N403
    N404(("="))
    N402 --> N404
    N405[initVal]
    N402 --> N405
    N406[exp]
    N405 --> N406
    N407[assignExp]
    N406 --> N407
    N408[lOrExp]
    N407 --> N408
    N409[lAndExp]
    N408 --> N409
    N410[eqExp]
    N409 --> N410
    N411[relExp]
    N410 --> N411
    N412[addExp]
    N411 --> N412
    N413[mulExp]
    N412 --> N413
    N414[unaryExp]
    N413 --> N414
    N415[primaryExp]
    N414 --> N415
    N416[number]
    N415 --> N416
    N417(("15"))
    N416 --> N417
    N418[varDef_list]
    N399 --> N418
    N419((";"))
    N399 --> N419
    N420[blockItem]
    N14 --> N420
    N421[stmt]
    N420 --> N421
    N422(("if"))
    N421 --> N422
    N423(("("))
    N421 --> N423
    N424[cond]
    N421 --> N424
    N425[lOrExp]
    N424 --> N425
    N426[lAndExp]
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
    N433[primaryExp]
    N432 --> N433
    N434(("("))
    N433 --> N434
    N435[exp]
    N433 --> N435
    N436[assignExp]
    N435 --> N436
    N437[lOrExp]
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
    N451[lAndExp]
    N450 --> N451
    N452[eqExp]
    N451 --> N452
    N453[relExp]
    N452 --> N453
    N454[relExp]
    N453 --> N454
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
    N460(("x"))
    N459 --> N460
    N461((">"))
    N453 --> N461
    N462[addExp]
    N453 --> N462
    N463[mulExp]
    N462 --> N463
    N464[unaryExp]
    N463 --> N464
    N465[primaryExp]
    N464 --> N465
    N466[number]
    N465 --> N466
    N467(("0"))
    N466 --> N467
    N468(("&&"))
    N450 --> N468
    N469[eqExp]
    N450 --> N469
    N470[relExp]
    N469 --> N470
    N471[relExp]
    N470 --> N471
    N472[addExp]
    N471 --> N472
    N473[mulExp]
    N472 --> N473
    N474[unaryExp]
    N473 --> N474
    N475[primaryExp]
    N474 --> N475
    N476[lVal]
    N475 --> N476
    N477(("y"))
    N476 --> N477
    N478((">"))
    N470 --> N478
    N479[addExp]
    N470 --> N479
    N480[mulExp]
    N479 --> N480
    N481[unaryExp]
    N480 --> N481
    N482[primaryExp]
    N481 --> N482
    N483[number]
    N482 --> N483
    N484(("0"))
    N483 --> N484
    N485((")"))
    N445 --> N485
    N486(("||"))
    N437 --> N486
    N487[lAndExp]
    N437 --> N487
    N488[eqExp]
    N487 --> N488
    N489[relExp]
    N488 --> N489
    N490[addExp]
    N489 --> N490
    N491[mulExp]
    N490 --> N491
    N492[unaryExp]
    N491 --> N492
    N493[primaryExp]
    N492 --> N493
    N494(("("))
    N493 --> N494
    N495[exp]
    N493 --> N495
    N496[assignExp]
    N495 --> N496
    N497[lOrExp]
    N496 --> N497
    N498[lAndExp]
    N497 --> N498
    N499[eqExp]
    N498 --> N499
    N500[relExp]
    N499 --> N500
    N501[relExp]
    N500 --> N501
    N502[addExp]
    N501 --> N502
    N503[mulExp]
    N502 --> N503
    N504[unaryExp]
    N503 --> N504
    N505[primaryExp]
    N504 --> N505
    N506[lVal]
    N505 --> N506
    N507(("z"))
    N506 --> N507
    N508((">"))
    N500 --> N508
    N509[addExp]
    N500 --> N509
    N510[mulExp]
    N509 --> N510
    N511[unaryExp]
    N510 --> N511
    N512[primaryExp]
    N511 --> N512
    N513[number]
    N512 --> N513
    N514(("20"))
    N513 --> N514
    N515((")"))
    N493 --> N515
    N516((")"))
    N433 --> N516
    N517(("&&"))
    N426 --> N517
    N518[eqExp]
    N426 --> N518
    N519[relExp]
    N518 --> N519
    N520[addExp]
    N519 --> N520
    N521[mulExp]
    N520 --> N521
    N522[unaryExp]
    N521 --> N522
    N523[primaryExp]
    N522 --> N523
    N524(("("))
    N523 --> N524
    N525[exp]
    N523 --> N525
    N526[assignExp]
    N525 --> N526
    N527[lOrExp]
    N526 --> N527
    N528[lAndExp]
    N527 --> N528
    N529[lAndExp]
    N528 --> N529
    N530[eqExp]
    N529 --> N530
    N531[relExp]
    N530 --> N531
    N532[addExp]
    N531 --> N532
    N533[mulExp]
    N532 --> N533
    N534[unaryExp]
    N533 --> N534
    N535[primaryExp]
    N534 --> N535
    N536(("("))
    N535 --> N536
    N537[exp]
    N535 --> N537
    N538[assignExp]
    N537 --> N538
    N539[lOrExp]
    N538 --> N539
    N540[lOrExp]
    N539 --> N540
    N541[lAndExp]
    N540 --> N541
    N542[eqExp]
    N541 --> N542
    N543[relExp]
    N542 --> N543
    N544[relExp]
    N543 --> N544
    N545[addExp]
    N544 --> N545
    N546[mulExp]
    N545 --> N546
    N547[unaryExp]
    N546 --> N547
    N548[primaryExp]
    N547 --> N548
    N549[lVal]
    N548 --> N549
    N550(("x"))
    N549 --> N550
    N551(("<"))
    N543 --> N551
    N552[addExp]
    N543 --> N552
    N553[mulExp]
    N552 --> N553
    N554[unaryExp]
    N553 --> N554
    N555[primaryExp]
    N554 --> N555
    N556[number]
    N555 --> N556
    N557(("10"))
    N556 --> N557
    N558(("||"))
    N539 --> N558
    N559[lAndExp]
    N539 --> N559
    N560[eqExp]
    N559 --> N560
    N561[relExp]
    N560 --> N561
    N562[relExp]
    N561 --> N562
    N563[addExp]
    N562 --> N563
    N564[mulExp]
    N563 --> N564
    N565[unaryExp]
    N564 --> N565
    N566[primaryExp]
    N565 --> N566
    N567[lVal]
    N566 --> N567
    N568(("y"))
    N567 --> N568
    N569(("<"))
    N561 --> N569
    N570[addExp]
    N561 --> N570
    N571[mulExp]
    N570 --> N571
    N572[unaryExp]
    N571 --> N572
    N573[primaryExp]
    N572 --> N573
    N574[number]
    N573 --> N574
    N575(("5"))
    N574 --> N575
    N576((")"))
    N535 --> N576
    N577(("&&"))
    N528 --> N577
    N578[eqExp]
    N528 --> N578
    N579[eqExp]
    N578 --> N579
    N580[relExp]
    N579 --> N580
    N581[addExp]
    N580 --> N581
    N582[mulExp]
    N581 --> N582
    N583[unaryExp]
    N582 --> N583
    N584[primaryExp]
    N583 --> N584
    N585[lVal]
    N584 --> N585
    N586(("z"))
    N585 --> N586
    N587(("!="))
    N578 --> N587
    N588[relExp]
    N578 --> N588
    N589[addExp]
    N588 --> N589
    N590[mulExp]
    N589 --> N590
    N591[unaryExp]
    N590 --> N591
    N592[primaryExp]
    N591 --> N592
    N593[number]
    N592 --> N593
    N594(("0"))
    N593 --> N594
    N595((")"))
    N523 --> N595
    N596((")"))
    N421 --> N596
    N597[stmt]
    N421 --> N597
    N598[block]
    N597 --> N598
    N599(("{"))
    N598 --> N599
    N600[blockItem_list]
    N598 --> N600
    N601[blockItem_list]
    N600 --> N601
    N602[blockItem]
    N600 --> N602
    N603[decl]
    N602 --> N603
    N604[varDecl]
    N603 --> N604
    N605[bType]
    N604 --> N605
    N606(("int"))
    N605 --> N606
    N607[varDef]
    N604 --> N607
    N608(("temp"))
    N607 --> N608
    N609(("="))
    N607 --> N609
    N610[initVal]
    N607 --> N610
    N611[exp]
    N610 --> N611
    N612[assignExp]
    N611 --> N612
    N613[lOrExp]
    N612 --> N613
    N614[lAndExp]
    N613 --> N614
    N615[eqExp]
    N614 --> N615
    N616[relExp]
    N615 --> N616
    N617[addExp]
    N616 --> N617
    N618[mulExp]
    N617 --> N618
    N619[unaryExp]
    N618 --> N619
    N620[primaryExp]
    N619 --> N620
    N621[number]
    N620 --> N621
    N622(("1"))
    N621 --> N622
    N623[varDef_list]
    N604 --> N623
    N624((";"))
    N604 --> N624
    N625(("}"))
    N598 --> N625
    N626[else_opt]
    N421 --> N626
    N627[blockItem]
    N13 --> N627
    N628[stmt]
    N627 --> N628
    N629(("return"))
    N628 --> N629
    N630[exp_opt]
    N628 --> N630
    N631[exp]
    N630 --> N631
    N632[assignExp]
    N631 --> N632
    N633[lOrExp]
    N632 --> N633
    N634[lAndExp]
    N633 --> N634
    N635[eqExp]
    N634 --> N635
    N636[relExp]
    N635 --> N636
    N637[addExp]
    N636 --> N637
    N638[mulExp]
    N637 --> N638
    N639[unaryExp]
    N638 --> N639
    N640[primaryExp]
    N639 --> N640
    N641[number]
    N640 --> N641
    N642(("0"))
    N641 --> N642
    N643((";"))
    N628 --> N643
    N644(("}"))
    N11 --> N644
    N645(("}"))
    N1 --> N645
```
