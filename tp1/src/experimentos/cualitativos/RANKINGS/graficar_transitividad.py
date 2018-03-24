#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt


#CMM
y = [100.0,100.0,100.0,100.0,100.0,75.47169811320755,61.53846153846154,59.45945945945946,55.223880597014926,53.80116959064327,46.3768115942029,47.23404255319149,46.54545454545455,46.03174603174603,40.992167101827675,43.18181818181818,37.199124726477024,42.65873015873016,39.51473136915078,34.95145631067961,34.55098934550989,38.482384823848236,37.365591397849464,36.37426900584796,35.567567567567565,36.004056795131845,36.26893939393939,38.475499092558984,38.553259141494436,40.53468208092485,39.36682725395733,34.87773487773488,35.126964933494556,37.172177879133415,33.31559340074507,37.644690488173126,36.51877133105802,33.43934575193094,31.87108325872874,34.69387755102041,36.0032693093584,34.74446987032799,35.498320268757,36.21908127208481,34.46536650975118,37.1015445284259,37.48431618569636,36.66768947529917,34.91416933372127,35.44050343249428,37.70942048887668,36.33930960663634,35.2211016291699,35.36495453428361,36.62312530237058,34.43693168570762,34.47649330002271,37.951937648841735,36.6008911521324,37.93592862935929,37.27399165507649,34.716198378276445,34.012597258243794,34.34639326254119,35.333333333333336,34.71933471933472,34.206523576136554,34.70935130581297,35.77966655769859,36.00064662140317,36.90494893951296,36.71178099046447,35.7505222321695,37.29138975040615,35.82283977420755,35.06803198204517,35.136995731791274,34.47771124417831,35.467980295566505,35.672290388548056,35.951849094068,35.569403714565006,35.620799434029,36.40381484066062,38.11359256320145,36.38258198968959,36.71638564634673,37.12668972021377,36.17764984064974,36.423372422159325,34.12406947890819,34.7577222934965,34.7304805536602,35.0,35.48326964302358,34.15672125080951,33.815290077715524,34.555863672958225,35.63208293068611,36.07360908074641,36.33807616937889,35.5840479560403,35.52816610141451,35.72454516437919,35.53824137382732,35.940218250830306,35.11693029592068,34.40707169076335,35.167080110900336,34.846846846846844,34.8195177628763,34.520221490152096,34.72962680883473,36.266196323180246,36.546426185704746,37.12806396644383,37.795939547253035,36.408142231383664,37.227660246289865,37.86201288546944,38.643901240633824,38.40207742013407,37.53344829636677,36.772641952135146,37.542840546035436,37.51000800640513,37.05285942128047,35.591517857142854,36.91109526682389,37.29753234047179,36.32037641020157,37.15748904492899]
x = [2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133] 

plt.errorbar(x, y, color='b')

#WP
y = [100.0,100.0,100.0,88.0,100.0,73.58490566037736,56.41025641025641,61.261261261261254,58.2089552238806,49.122807017543856,37.19806763285024,37.87234042553192,35.63636363636364,42.857142857142854,41.77545691906005,47.474747474747474,38.7308533916849,43.452380952380956,41.074523396880416,38.673139158576056,32.87671232876712,39.43089430894309,40.053763440860216,39.06432748538012,37.08108108108108,35.598377281947265,36.26893939393939,39.56442831215971,39.7456279809221,41.40173410404625,37.16448726772196,36.16473616473617,38.21039903264813,36.60205245153934,36.56200106439596,36.88978359335682,41.101901511457825,38.75511131303953,37.60071620411817,37.92517006802721,36.24846751123825,36.49885583524027,36.73012318029115,38.515901060070675,34.59986550100874,35.524153795596455,35.633626097867,34.94937097269101,36.892638929298805,36.61327231121281,39.54957429277671,38.64062081883864,38.531161106801136,35.85647579257803,38.05031446540881,34.87992539053392,35.63479445832387,38.10348560294436,35.83704646721833,37.57096512570965,36.91635207629644,33.79219309824627,32.10448314190441,33.614060783595754,35.07017543859649,34.16493416493417,34.10512083826263,35.585509688289804,34.70088264138607,36.27546071774976,37.32914375490966,36.019686250384495,35.78036407042674,34.854526657805344,35.63467940367636,34.899705428531355,34.696406443618336,33.41317365269461,33.43271973036038,34.355828220858896,35.91461901216182,35.87487781036168,38.497818653460676,39.39288206559665,39.08853871443147,38.38982121311835,37.224383916990924,38.70900136225506,38.223501593502625,38.6372826526486,36.83374689826302,36.04065923221159,35.56876888585632,36.25478927203065,36.070463230496784,35.75723933758905,36.12868245075004,35.593523570981304,35.737503294386364,36.047811505718464,36.01431370878419,35.24269419698609,35.28000661758624,36.219278646664534,36.06296708538718,36.533291159259846,34.69310527510785,35.238594651284735,35.50269954764337,35.87747747747748,35.846768440576405,35.55758043036378,36.07976182233608,36.578251136286546,36.71342685370742,36.49888583038406,37.43270415774794,37.43880443184746,37.99179033785917,38.87533621067117,38.86500429922614,38.92143245365058,37.95564012606291,37.69943688409197,37.33952948010456,36.89237103968889,37.092731829573935,37.37723214285714,38.282235507047666,38.60745733231873,38.34678928514142,38.61992503035743]
x = [2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133] 

plt.errorbar(x, y,  color='g')

z = [37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109,37.711407109]
plt.errorbar(x, z,  color='r')

plt.ylim(0,105)
plt.xlim(0,134)
plt.title('Experimento 4: Transitividad')
plt.legend(('CMM','WP'))
plt.xlabel('Fechas')
plt.ylabel('Porcentaje de transitividad a favor')
#plt.grid(True)
plt.show()

#Promedio de CMM : 39.8790339156   Sin outliers: 37.5120667469
#Promedio de WP: 40.1717040063 	   Sin outliers: 37.9107474711
#Promedio de ambos:40.0-253689609   Sin outliers: 37.711407109


