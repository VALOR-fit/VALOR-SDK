//____________________________________________________________________________
/*!

\class     valor::conv::PhysParamId_t;

\brief     Conventions

\author    Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
           University of Liverpool & STFC Rutherford Appleton Laboratory
          
\created   Oct 20, 2010

\copyright (c) 2010-2016, The VALOR group

*/
//____________________________________________________________________________

#ifndef _phys_param_id_h_
#define _phys_param_id_h_

#include <string>

using std::string;

namespace valor {

namespace conv {

// Max number of fit  parameters listed below (for booking large enough param arrays)
static const int knppmax = 390;

typedef enum EPhysParamId 
{   
   kPhypUndefined = -1, 

   /*
    * List of oscillation parameters (various schemes)
    * ------------------------------------------------
   */

   kPhypMixing23,              // 23-mixing parameter (either sin^2(2*\theta_{23}) or sin^2(\theta_{23}) depending on config choices)
   kPhypMixing13,              // 13-mixing parameter (either sin^2(2*\theta_{13}) or sin^2(\theta_{13}) depending on config choices)
   kPhypMixing12,              // 12-mixing parameter (either sin^2(2*\theta_{12}) or sin^2(\theta_{12}) depending on config choices)
   kPhypCPV,                   // CP-invariance violating param (either deltacp, or sin(deltacp) depending on config choices)
   kPhypDm23sq,                // modulus of large squared-mass splitting (Dm^2_32, Dm^2_31 or Fogli-Lisi Dm^2 depending on config choices)
   kPhypDm12sq,                // small squared-mass splitting (Dm^2_21 or Fogli-Lisi dm^2 depending on config choices)

   // Antineutrino mixing parameters, used only for hypotheses when neutrinos and antineutrinos are given different parameters.
   kPhypMixing23Bar,           // 23-mixing parameter (either sin^2(2*\theta_{23}) or sin^2(\theta_{23}) depending on config choices)
   kPhypMixing13Bar,           // 13-mixing parameter (either sin^2(2*\theta_{13}) or sin^2(\theta_{13}) depending on config choices)
   kPhypMixing12Bar,           // 12-mixing parameter (either sin^2(2*\theta_{12}) or sin^2(\theta_{12}) depending on config choices)
   kPhypCPVBar,                // CP-invariance violating param (either deltacp, or sin(deltacp) depending on config choices)
   kPhypDm23sqBar,             // modulus of large squared-mass splitting (Dm^2_32, Dm^2_31 or Fogli-Lisi Dm^2 depending on config choices)
   kPhypDm12sqBar,             // small squared-mass splitting (Dm^2_21 or Fogli-Lisi dm^2 depending on config choices)                     
   
   // NEW BETABAR
   kPhypBetaBar,               // Normalization parameter for oscillated numubar->nuebar events (used in searches for numubar->nuebar appearance) 
   //

   kPhypAlphaDecay,            //
   kPhypSinsq2ThetaDecay,      // 
   kPhypMusqDecoh,             //
   kPhypSinsq2ThetaDecoh,      //
   kPhypMixing14,              // Sterile Mixings
   kPhypMixing15,              // Sterile Mixings
   kPhypMixing24,              // Sterile Mixings
   kPhypMixing25,              // Sterile Mixings
   kPhypMixing34,              // Sterile Mixings
   kPhypMixing35,              // Sterile Mixings
   kPhypDm41sq,                // Sterile mass splitting
   kPhypDm51sq,                // Sterile mass splitting
   kPhypDelta12,               // Sterile phase
   kPhypDelta13,               // Sterile phase - same as kPhypDeltaCP, but separate for constistency.
   kPhypDelta15,               // Sterile phase
   kPhypDelta34,               // Sterile phase
   kPhypDelta35,               // Sterile phase
   kPhypNSIepsMuTau,           // Nonstandard matter interactions
   
   
   // Generic nuisance parameters
  kNuis000 ,
  kNuis001 ,
  kNuis002 ,
  kNuis003 ,
  kNuis004 ,
  kNuis005 ,
  kNuis006 ,
  kNuis007 ,
  kNuis008 ,
  kNuis009 ,
  kNuis010 ,
  kNuis011 ,
  kNuis012 ,
  kNuis013 ,
  kNuis014 ,
  kNuis015 ,
  kNuis016 ,
  kNuis017 ,
  kNuis018 ,
  kNuis019 ,
  kNuis020 ,
  kNuis021 ,
  kNuis022 ,
  kNuis023 ,
  kNuis024 ,
  kNuis025 ,
  kNuis026 ,
  kNuis027 ,
  kNuis028 ,
  kNuis029 ,
  kNuis030 ,
  kNuis031 ,
  kNuis032 ,
  kNuis033 ,
  kNuis034 ,
  kNuis035 ,
  kNuis036 ,
  kNuis037 ,
  kNuis038 ,
  kNuis039 ,
  kNuis040 ,
  kNuis041 ,
  kNuis042 ,
  kNuis043 ,
  kNuis044 ,
  kNuis045 ,
  kNuis046 ,
  kNuis047 ,
  kNuis048 ,
  kNuis049 ,
  kNuis050 ,
  kNuis051 ,
  kNuis052 ,
  kNuis053 ,
  kNuis054 ,
  kNuis055 ,
  kNuis056 ,
  kNuis057 ,
  kNuis058 ,
  kNuis059 ,
  kNuis060 ,
  kNuis061 ,
  kNuis062 ,
  kNuis063 ,
  kNuis064 ,
  kNuis065 ,
  kNuis066 ,
  kNuis067 ,
  kNuis068 ,
  kNuis069 ,
  kNuis070 ,
  kNuis071 ,
  kNuis072 ,
  kNuis073 ,
  kNuis074 ,
  kNuis075 ,
  kNuis076 ,
  kNuis077 ,
  kNuis078 ,
  kNuis079 ,
  kNuis080 ,
  kNuis081 ,
  kNuis082 ,
  kNuis083 ,
  kNuis084 ,
  kNuis085 ,
  kNuis086 ,
  kNuis087 ,
  kNuis088 ,
  kNuis089 ,
  kNuis090 ,
  kNuis091 ,
  kNuis092 ,
  kNuis093 ,
  kNuis094 ,
  kNuis095 ,
  kNuis096 ,
  kNuis097 ,
  kNuis098 ,
  kNuis099 ,
  kNuis100 ,
  kNuis101 ,
  kNuis102 ,
  kNuis103 ,
  kNuis104 ,
  kNuis105 ,
  kNuis106 ,
  kNuis107 ,
  kNuis108 ,
  kNuis109 ,
  kNuis110 ,
  kNuis111 ,
  kNuis112 ,
  kNuis113 ,
  kNuis114 ,
  kNuis115 ,
  kNuis116 ,
  kNuis117 ,
  kNuis118 ,
  kNuis119 ,
  kNuis120 ,
  kNuis121 ,
  kNuis122 ,
  kNuis123 ,
  kNuis124 ,
  kNuis125 ,
  kNuis126 ,
  kNuis127 ,
  kNuis128 ,
  kNuis129 ,
  kNuis130 ,
  kNuis131 ,
  kNuis132 ,
  kNuis133 ,
  kNuis134 ,
  kNuis135 ,
  kNuis136 ,
  kNuis137 ,
  kNuis138 ,
  kNuis139 ,
  kNuis140 ,
  kNuis141 ,
  kNuis142 ,
  kNuis143 ,
  kNuis144 ,
  kNuis145 ,
  kNuis146 ,
  kNuis147 ,
  kNuis148 ,
  kNuis149 ,
  kNuis150 ,
  kNuis151 ,
  kNuis152 ,
  kNuis153 ,
  kNuis154 ,
  kNuis155 ,
  kNuis156 ,
  kNuis157 ,
  kNuis158 ,
  kNuis159 ,
  kNuis160 ,
  kNuis161 ,
  kNuis162 ,
  kNuis163 ,
  kNuis164 ,
  kNuis165 ,
  kNuis166 ,
  kNuis167 ,
  kNuis168 ,
  kNuis169 ,
  kNuis170 ,
  kNuis171 ,
  kNuis172 ,
  kNuis173 ,
  kNuis174 ,
  kNuis175 ,
  kNuis176 ,
  kNuis177 ,
  kNuis178 ,
  kNuis179 ,
  kNuis180 ,
  kNuis181 ,
  kNuis182 ,
  kNuis183 ,
  kNuis184 ,
  kNuis185 ,
  kNuis186 ,
  kNuis187 ,
  kNuis188 ,
  kNuis189 ,
  kNuis190 ,
  kNuis191 ,
  kNuis192 ,
  kNuis193 ,
  kNuis194 ,
  kNuis195 ,
  kNuis196 ,
  kNuis197 ,
  kNuis198 ,
  kNuis199 ,
  kNuis200 ,


  kNuis201 ,
  kNuis202 ,
  kNuis203 ,
  kNuis204 ,
  kNuis205 ,
  kNuis206 ,
  kNuis207 ,
  kNuis208 ,
  kNuis209 ,
  kNuis210 ,
  kNuis211 ,
  kNuis212 ,
  kNuis213 ,
  kNuis214 ,
  kNuis215 ,
  kNuis216 ,
  kNuis217 ,
  kNuis218 ,
  kNuis219 ,
  kNuis220 ,

  kNuis221 ,
  kNuis222 ,
  kNuis223 ,
  kNuis224 ,
  kNuis225 ,
  kNuis226 ,
  kNuis227 ,
  kNuis228 ,
  kNuis229 ,
  kNuis230 ,
  kNuis231 ,
  kNuis232 ,
  kNuis233 ,
  kNuis234 ,
  kNuis235 ,
  kNuis236 ,
  kNuis237 ,
  kNuis238 ,
  kNuis239 ,
  kNuis240 ,
  kNuis241 ,
  kNuis242 ,
  kNuis243 ,
  kNuis244 ,
  kNuis245 ,
  kNuis246 ,
  kNuis247 ,
  kNuis248 ,
  kNuis249 ,
  kNuis250 ,
    
    kNuis251 ,
    kNuis252 ,
    kNuis253 ,
    kNuis254 ,
    kNuis255 ,
    kNuis256 ,
    kNuis257 ,
    kNuis258 ,
    kNuis259 ,
    kNuis260 ,
    kNuis261 ,
    kNuis262 ,
    kNuis263 ,
    kNuis264 ,
    kNuis265 ,
    kNuis266 ,
    kNuis267 ,
    kNuis268 ,
    kNuis269 ,
    kNuis270 ,
    kNuis271 ,
    kNuis272 ,
    kNuis273 ,
    kNuis274 ,
    kNuis275 ,
    kNuis276 ,
    kNuis277 ,
    kNuis278 ,
    kNuis279 ,
    kNuis280 ,
    kNuis281 ,
    kNuis282 ,
    kNuis283 ,
    kNuis284 ,
    kNuis285 ,
    kNuis286 ,
    kNuis287 ,
    kNuis288 ,
    kNuis289 ,
    kNuis290 ,
    kNuis291 ,
    kNuis292 ,
    kNuis293 ,
    kNuis294 ,
    kNuis295 ,
    kNuis296 ,
    kNuis297 ,
    kNuis298 ,
    kNuis299 ,
    kNuis300 ,
    
    
    kNuis301 ,
    kNuis302 ,
    kNuis303 ,
    kNuis304 ,
    kNuis305 ,
    kNuis306 ,
    kNuis307 ,
    kNuis308 ,
    kNuis309 ,
    kNuis310 ,
    kNuis311 ,
    kNuis312 ,
    kNuis313 ,
    kNuis314 ,
    kNuis315 ,
    kNuis316 ,
    kNuis317 ,
    kNuis318 ,
    kNuis319 ,
    kNuis320 ,
    
    kNuis321 ,
    kNuis322 ,
    kNuis323 ,
    kNuis324 ,
    kNuis325 ,
    kNuis326 ,
    kNuis327 ,
    kNuis328 ,
    kNuis329 ,
    kNuis330 ,
    kNuis331 ,
    kNuis332 ,
    kNuis333 ,
    kNuis334 ,
    kNuis335 ,
    kNuis336 ,
    kNuis337 ,
    kNuis338 ,
    kNuis339 ,
    kNuis340 ,
    kNuis341 ,
    kNuis342 ,
    kNuis343 ,
    kNuis344 ,
    kNuis345 ,
    kNuis346 ,
    kNuis347 ,
    kNuis348 ,
    kNuis349 ,
    kNuis350 ,
   
  kLastNuisParam
}
PhysParamId_t;

} // conv

namespace util {

  string RawParamAsStringRaw    (conv::PhysParamId_t p);
  string RawParamAsString       (conv::PhysParamId_t p);
  string RawParamAsStringTex    (conv::PhysParamId_t p);
  string RawParamAsStringRooTex (conv::PhysParamId_t p);
  
  conv::PhysParamId_t PhysParamIdFromRawString(string sh);

  bool IsSystematic   (conv::PhysParamId_t p);
  bool IsMixingAngle  (conv::PhysParamId_t p);
  bool IsMassSplitting(conv::PhysParamId_t p);
  bool IsPhase        (conv::PhysParamId_t p);
  bool IsNormalization(conv::PhysParamId_t p);

} // util

} // valor
  
#endif // _phys_param_id_h_

