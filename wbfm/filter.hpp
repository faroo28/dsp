#include "../fir.hpp"

// Lowpass at 96 kHz (fs = 2.4 MHz)
constexpr dsp::FIRFilter<128> wbfm_filter = {{{
    -0,
    -1.619747877e-06,
    -5.510018582e-06,
    -9.303685147e-06,
    -9.767926713e-06,
    -3.369712886e-06,
    1.303640147e-05,
    4.149890447e-05,
    8.228111983e-05,
    0.0001333287801,
    0.0001899713679,
    0.0002448770683,
    0.000288461626,
    0.0003096036089,
    0.0002968099725,
    0.0002396686468,
    0.000130483706,
    -3.399305569e-05,
    -0.0002509263577,
    -0.0005104041193,
    -0.0007946043042,
    -0.001078139991,
    -0.001329120481,
    -0.001511430368,
    -0.001587552717,
    -0.001522556646,
    -0.001288570347,
    -0.0008691488183,
    -0.0002634992998,
    0.0005102108116,
    0.001412752899,
    0.002383745043,
    0.003343723714,
    0.004197987262,
    0.004843229428,
    0.0051762769,
    0.005103227217,
    0.004550547805,
    0.003475463716,
    0.001874987036,
    -0.0002070412302,
    -0.002675369615,
    -0.005383542273,
    -0.008137937635,
    -0.01070581283,
    -0.01282833982,
    -0.01423651818,
    -0.01467022114,
    -0.01389780827,
    -0.01173649356,
    -0.00807021372,
    -0.002863834379,
    0.003826325061,
    0.01184852887,
    0.02095889114,
    0.03082983382,
    0.04106620327,
    0.05122586712,
    0.06084575877,
    0.06946905702,
    0.07667387277,
    0.08209969848,
    0.08547080308,
    0.08661417663,
    0.08547080308,
    0.08209969848,
    0.07667387277,
    0.06946905702,
    0.06084575877,
    0.05122586712,
    0.04106620327,
    0.03082983382,
    0.02095889114,
    0.0118485298,
    0.003826325061,
    -0.002863834379,
    -0.00807021372,
    -0.01173649356,
    -0.0138978092,
    -0.01467022114,
    -0.01423651725,
    -0.01282834169,
    -0.0107058147,
    -0.008137937635,
    -0.005383541808,
    -0.002675370313,
    -0.0002070412302,
    0.001874987036,
    0.003475463018,
    0.00455054827,
    0.005103227682,
    0.0051762769,
    0.004843228497,
    0.004197988659,
    0.003343723947,
    0.002383745043,
    0.001412753598,
    0.000510210928,
    -0.0002634993289,
    -0.0008691487019,
    -0.001288571046,
    -0.001522557111,
    -0.00158755295,
    -0.001511429669,
    -0.001329120831,
    -0.001078139991,
    -0.0007946044789,
    -0.0005104038282,
    -0.0002509264159,
    -3.399307025e-05,
    0.000130483706,
    0.0002396685304,
    0.0002968101471,
    0.0003096038126,
    0.000288461626,
    0.0002448773303,
    0.0001899713679,
    0.0001333287801,
    8.228111983e-05,
    4.149890447e-05,
    1.303635509e-05,
    -3.369712886e-06,
    -9.767926713e-06,
    -9.303685147e-06,
    -5.509835773e-06,
    -1.619747877e-06,
    -0,
    0
}}};

// Lowpass at 20 kHz (fs = 2.4 MHz)
constexpr dsp::FIRFilter<192> audio_filter = {{{
    -0,
    -5.141214388e-07,
    -2.050357807e-06,
    -4.592394362e-06,
    -8.115262972e-06,
    -1.25842862e-05,
    -1.79556082e-05,
    -2.417416727e-05,
    -3.117511005e-05,
    -3.888096762e-05,
    -4.719979552e-05,
    -5.602538295e-05,
    -6.523873162e-05,
    -7.470760465e-05,
    -8.427003922e-05,
    -9.376081289e-05,
    -0.0001029836931,
    -0.0001117308711,
    -0.0001197638121,
    -0.0001268331253,
    -0.0001326493366,
    -0.0001369329111,
    -0.0001393306738,
    -0.0001395356521,
    -0.0001371257822,
    -0.000131740424,
    -0.0001229678455,
    -0.0001103534087,
    -9.34632335e-05,
    -7.181453839e-05,
    -4.490300853e-05,
    -1.225522374e-05,
    2.669397327e-05,
    7.237619138e-05,
    0.0001254365634,
    0.0001862780919,
    0.0002555697283,
    0.0003336845257,
    0.0004212713975,
    0.0005187811912,
    0.0006266643759,
    0.0007454525912,
    0.0008755115559,
    0.00101732614,
    0.001171231852,
    0.001337514492,
    0.001516555669,
    0.001708499738,
    0.001913638785,
    0.00213194103,
    0.002363706008,
    0.002608898096,
    0.002867206,
    0.003138824832,
    0.003423362272,
    0.003720687004,
    0.004030352924,
    0.004351973999,
    0.004685056861,
    0.005028989166,
    0.005383286159,
    0.005747023504,
    0.006119443569,
    0.006499763578,
    0.00688700797,
    0.007280174177,
    0.007678203285,
    0.008080014959,
    0.008484350517,
    0.008890029974,
    0.009295794182,
    0.009700337425,
    0.0101022888,
    0.01050046738,
    0.01089324336,
    0.01127941068,
    0.01165753976,
    0.01202623453,
    0.01238421444,
    0.01273001172,
    0.01306238957,
    0.01338006556,
    0.01368176565,
    0.01396632008,
    0.01423261128,
    0.01447953191,
    0.01470611431,
    0.01491144672,
    0.01509467047,
    0.01525502931,
    0.01539186388,
    0.01550460886,
    0.01559279021,
    0.01565604471,
    0.01569410227,
    0.01570680737,
    0.01569410227,
    0.01565604471,
    0.01559279021,
    0.01550460886,
    0.01539186202,
    0.01525502931,
    0.01509466954,
    0.01491144579,
    0.01470611431,
    0.01447953191,
    0.01423261035,
    0.01396631822,
    0.01368176378,
    0.01338006277,
    0.0130623877,
    0.01273000985,
    0.01238421258,
    0.01202623453,
    0.01165753882,
    0.01127940696,
    0.0108932415,
    0.01050046459,
    0.010102286,
    0.009700335562,
    0.009295790456,
    0.008890028112,
    0.008484347723,
    0.008080014028,
    0.007678200956,
    0.007280173246,
    0.006887005176,
    0.006499763113,
    0.00611944031,
    0.005747019779,
    0.005383284297,
    0.005028986838,
    0.004685055465,
    0.004351972137,
    0.004030351993,
    0.003720685141,
    0.003423361108,
    0.003138823435,
    0.002867203904,
    0.002608896932,
    0.002363704145,
    0.002131940331,
    0.001913637621,
    0.001708499272,
    0.001516554737,
    0.001337514026,
    0.001171231153,
    0.001017325325,
    0.0008755111485,
    0.0007454518927,
    0.0006266640848,
    0.0005187806673,
    0.0004212712811,
    0.0003336842055,
    0.0002555696119,
    0.0001862779172,
    0.000125436447,
    7.237614773e-05,
    2.669394235e-05,
    -1.225521464e-05,
    -4.490296487e-05,
    -7.181449473e-05,
    -9.346314619e-05,
    -0.0001103533432,
    -0.0001229677582,
    -0.0001317402639,
    -0.000137125724,
    -0.0001395354484,
    -0.0001393306011,
    -0.0001369327656,
    -0.000132649162,
    -0.0001268329361,
    -0.0001197638121,
    -0.0001117306674,
    -0.0001029834748,
    -9.376070375e-05,
    -8.426979184e-05,
    -7.470760465e-05,
    -6.523860065e-05,
    -5.602538295e-05,
    -4.719965727e-05,
    -3.888096762e-05,
    -3.117496453e-05,
    -2.417401993e-05,
    -1.795530807e-05,
    -1.25842862e-05,
    -8.115111086e-06,
    -4.592394362e-06,
    -2.050357807e-06,
    -5.14277076e-07,
    -0,
    0
}}};
