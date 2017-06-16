/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_myNeuralNetworkFunction_info.c
 *
 * Code generation for function '_coder_myNeuralNetworkFunction_info'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "myNeuralNetworkFunction.h"
#include "_coder_myNeuralNetworkFunction_info.h"
#include "myNeuralNetworkFunction_data.h"
#include "blas.h"

/* Function Definitions */
mxArray *emlrtMexFcnProperties(void)
{
  mxArray *xResult;
  mxArray *xEntryPoints;
  const char * fldNames[4] = { "Name", "NumberOfInputs", "NumberOfOutputs",
    "ConstantInputs" };

  mxArray *xInputs;
  const char * b_fldNames[4] = { "Version", "ResolvedFunctions", "EntryPoints",
    "CoverageInfo" };

  xEntryPoints = emlrtCreateStructMatrix(1, 1, 4, fldNames);
  xInputs = emlrtCreateLogicalMatrix(1, 1);
  emlrtSetField(xEntryPoints, 0, "Name", mxCreateString(
    "myNeuralNetworkFunction"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs", mxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs", mxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  xResult = emlrtCreateStructMatrix(1, 1, 4, b_fldNames);
  emlrtSetField(xResult, 0, "Version", mxCreateString("9.0.0.341360 (R2016a)"));
  emlrtSetField(xResult, 0, "ResolvedFunctions", (mxArray *)
                emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  emlrtSetField(xResult, 0, "CoverageInfo", covrtSerializeInstanceData
                (&emlrtCoverageInstance));
  return xResult;
}

const mxArray *emlrtMexFcnResolvedFunctionsInfo(void)
{
  const mxArray *nameCaptureInfo;
  const char * data[27] = {
    "789ced5dcd6fdcc6151f35b69ba248e2142892346d6ae5102435aa7563a02d7a2824cb72b48e3f644b7214bb2ec52567b5ac480e4172ed552e5d2440e1a24051"
    "d4fdc8a9f0b1b9f4d443fe851e8aa2450ae416f44f285aa0bd1428875cee92b3d4ce889cd9a5564fc05a7eab99e17bf3de6fde7bf345b4d0bc8ea29f67a3cfee",
    "0b0be84cf4fbe9e8f33994fc9c1ed00bd1e7dee077f2fd29f4cc807e3ffa18c40d712f4cfee8ea0e46e98f491ccbd5dd70ebc0c3c8c701b11f6033fe4bdbb2f1"
    "96e5e06b2443ac5b11e15cc9fc6948d03ff99d60d832b2b344f243e5e8459f548e530572bc9591e3ec80beb7767ff57b8ded00fb41e38aafbb060e0cd2b88c83",
    "fd90780de7e006eefaba7d03870f89bf7fa5eb1aa145dc2567d1d1bd483c47ef69bae7d907f1f3bfcb79fe19e6f9946e05bd76d74df85fe6d47f8ea94fe9f680"
    "25ada3bba68de376fa9c76ae32ed50fa5ef3da4edc151b3ed9f375e71cedfca0717d65ebdacaa5c6ed372f7cebdb7a2324c46e915e033b76c3b65a0d470f6dbd",
    "d5c076f49f4622c99233d247b63fce14f0b190e1e30b83ef23ee7f73e9a5bfaf94af5ff5f9f2eabf9ea9bf50501f657e9729ff0b3459cf3f40793d535a9e9e17"
    "93ff6806713c3db45a364ef9dae0f0f50ac317a5a34769961b6a6de2db84781a7980fdb64d1e6a46071bfb49bb1726b49bfe64db15eda75da6de6ed57e8ac8c9",
    "f22c3915f0f15bc08758f98fd164bdff04e5f54e69d57a5f9c5c40eb60dbc33edfde1706df8df83f85aca06dd17aeb9c7a2f307253da2026f69722c6b0efea76"
    "f41f13f79a6e28e457af30ed5d29db8f6dab874d8f445c34a82cd49754b0b35f034ec4cacf5abfcb9ce73fc33c9fd256e0761dec5b4648c3cb7278313aba2f12",
    "2fdd649e7f538afc19fea38e003b077fc0f30765f28ba8e92849518b0f35f944c279f57c0241bc2458fea4e2c3728f2b3e2c09f936e043b43c2f8fd4505ecf94"
    "96986fbb44330f5cddb10c0df73cdd0d2ce242be0df9766df0d147b31807c7e71d79e3f853393e9e423440529137f3faa3c9b4d7acd81fa61eea515734326153",
    "15bbf9f4df2db07b19763f2d3df3ecf745860f4a33f6ebeafe9ee51a9d7d95e37fe579e9200abcb0d988791ffc7b7ef02b95a4319424e9a20ae3ff8f5fbffd09"
    "e040a0fc71d07b115fa719be281dc50ec417ac5f945798a41baf88cc627cc0361d1d620964f8818f965f033f50473f70989e79f9c0d7193e28cdc631c1a5ae65",
    "874df746324b2905078f38f57798fa3b65fa877ec60605569c2af900fae3b37f067f304f787899e183d20c1e0243b7757f29f50ad5f2631e0eb6997adb65faa5"
    "1007e71339327d54212efa25e4c5c70b07bb1c3ede60f8a034838378b3d3666c44e966a8a6bb61eb06ce3c6799f31cd17d4c1f72da6931edb4caf45b214e0e17",
    "53c27cebddcf2e026e44cad75dffaafc8aea380bfccb7ce1a48fa6eb5fcace3f09cc9f3a967bd3bf3e988f55b51eb7c1f0b151a63f0aed3f65bf527eb176d504"
    "bb9761f7d3d233cfee61de15e65d6739ef7a1fe5f54e69d93888f769e05ee863a79bfa7dc005e0a2cef9838ef27aa7b4125cb42c378b8d943f09f152126fafed",
    "cd64fd5a9d7f4dc5aa14479dfdf426c451f58ea3f27a5ee6f021ba0f7cda3880fde1800355e35d111f4f337c509a9ea788d2f3ce2cec5fdabe2ee67c089547c2"
    "7c2bd8bf60f94768b29ed5adcf8eec7fb16df941989c63837dadb0af1570318e0b38170afe00f26769eb6fb8e7add8363186edcac6096ffcd862dadb92d68f59",
    "f1aace3ba1c7801b39fe62dafadee1f0f32ac30fa527e0e406594d62a9b47dd978e1c559f798f6ee29e9bf81988304a4429cf55fe314e04686bfe9a0bcde29ad"
    "c4df246157ceedc07c2dccd7024ec470a26afff9b4e332d88f3e5fb8e9a3c9fa9ed6f95498d782792dc0c3880fd97195eaf37a10479d0cfb3f6eebdeaaed1ed6",
    "b9c1eeebb4ceaddade615d7b3eec7d76799edcf5bbaaf60eeb75f36de77d34a5f1ac627efb55860f4a1f324f731bdbc44bdbad6d7e2bb2ff3b2751a5fcf6af17"
    "5cc08348f98fd164bd7f80f27aa7b46abd2fb66da2879a61eb41a0d19767e8961b6869a155faf5907f1e8e5e62f8a73483a3f869d7a3efec4cbf943d57c7f3a3",
    "b7197e6e1fb53f0ff5a3233124bc5f02ce950a96ff3d9aacef1ecaeb9bd2caf163055a60edb9d8cc83a66c7c04f7bfc2fdafa2e53b68b29ebf83f27aa674c9f7"
    "0b959ecff1b11719c774e683d4d875228104bbf67ea7815dd7376f18d7f33a870f382f5a2ade81f3a282e5fba81e38e0edd75b64f8a0347b4f5310799cf08e6e",
    "5be6a6f51e5ef1f70ed14319bfc0c3c75da6fedd32fd547c2fcf985812f281bffcf032f80991f275d5bbaafd465571f288531ff617cd173e78fbf230caeb9bd2"
    "eaf01165caf4ef51a37641ff15f1779ae18fd2b491b660fdaaf984fc7b10e33c994a20e31e64c89305cbf7513df40c7e01fc421df05043bfd0225dd70c90184e",
    "bec6f047e9319c34a9afc17eba82a1729de10ec3cf1d69fd9517a3daba35ac33cc0b3ecaae2bc07b17c1fe4fb2fdc3ba1ad8ff3cd83fec6782fd4c75c0495de7"
    "5fc13f807f00bb1fe70bf202b0fb79b47b8887201eaa033efa6816e3e0f87e0dde380eef9b16b21b78dfb460f93eaa87ddc33d16708f05e061c407c445101701",
    "1e467c48886fe05e1734b41fb8d745b0fc54f2e643ed7fd126c665cb09567cbce678e1c1882f8897205eaa033efa686efc83e71333b0dec34ae753d5f98594fd"
    "4a7ee13f67d7601f9e48f90e9aac67d9e73779f6fd3cf33c4ad3f1d1092d07075a07db1ef633fc971de71f71f8d864ea51ba0ade891734c6e4a87e8ff68730be",
    "8b95e7eda7d0515edf9456a0ef4583380e7113cf1e64f8837dd9b02fbb0e38a9dbb828211e827c1941be7cdc71c0f30f02f71eb56c3d58eaed6127bb2ea5ea3c"
    "8294fe29c4c1792a472396a37afcf4f37fbc0f78a80d1e8ea06f1e1ebec2f043e9223c58ae6db97824a7aa7c42ddfb8d92fe49e4a8fc3eab57fed58738691ef1",
    "20903fc478e80658f3719bfe5fe93ceabb4cbd7725f74f460e19fb30001782e57979b681f27aa7b4025c2cb6309d400f3b3e0e3ac43687fc4938d719db54b665"
    "9571d43b0c3fef48eeafa11cd5ee99077c0896af9bdf58e7f003f935e4d727010712f629a5e1c630c556e917d49df73f3f9063d44515f2eb2ff6f7010f32f030",
    "6d7d4b986f8a77b95db7dc6e769d43f57a84bafbb647e24838eff3fdff05102f1d475c40bc04f112e0409e7fd8a2cb1d193925e0eb48efb355ec2f62f1c05f8c"
    "ea9f349cf0ecf945861f4a17d9f386dd0d66818f5b4c7bb7a4f5d7502c19f3b3800fc1f275c307ecff86fddf800bf57e4375de0d7ee264e1e118ae530cf67b84",
    "4af779a8c341bacf239480837e73fb00fc8248f972e7e67656ab1f175eeb258abf8c3d1c0d81ae717094f7a1bccaf04569060f4904b21a7db987dd75dd35edcc"
    "3e289ebff83cd33ea5bbeebe4b1eba42fda631f53594e044b8e3261eb02e106dc9ab8297bfed3c01bcc8f01bdb28af774a1f49ef9cf1f1d2b595cd152f7ebd72",
    "c5386a1c7fd3c0c5acc69392b8d8fbd34f219e1229ff044dd6fb8f505eef9496371e5e6d6e6d763d8ff821360becc01bf1097e05fc4a1df15337bfb2cbe1e79b"
    "0c3f946670321994057a298397279cfa751977003fb3c5cfb4f356debcefcb0c3f942ecae707b81cb6ab2aaf9ff6f852120fff3421af172a5f368e90bd4fddc0",
    "0f745b4bb60c8ae042601f628c0b7a8386e6857e2aaf2a5c289c178ffb2995a3da3e44d89f7edc71b1cee1ebcb0c5f9466706168861e84057a286aef34d31ea5"
    "a3962ebe89a6b40f71625c958802f1d3a83ee002f623c27ec4517dd578e8a1c97abe8cf27aa674c97ba31643dd0dac3d4df73cfb207d3ecfeee11ea923f901b8",
    "474ab0fcaced9e779e88bd571ff7c6e727cb8ceb3cb9df62ea53badafd88f416fd88fb6ae7523f5a7e0dec5a865dabd6af84fde44900b0442d7e2457adc7f5e2"
    "782f9163d037293f25c77578dfae60f959dbff2ee7f96f30cfa73463ffb19fd88c8d2775224d77c3d60d9c917399f39ce798e750ba3d684deb248b79b41dde3d",
    "0f2da69d5699fe2a7e0fd3a1624a386771f7b38b801791f275d7bf847b3eba01ded0fd36f157893b9c373a7ee7b9f362543ccf7dee0fdf007c8894af9bbed739"
    "fc08ec271f36ac32ae52b70e39645fc6fed9fbbffa00707012fc049c3f82f347801375f9b9ea734990a7cf174e663dff5a663f9e6f5a0f2c134f657dadc9d4a7",
    "74d5f58601ff32e2a6c7676e819d8b94efa37ae89937eec37b29e0bd148087111fe7183e285d10074541c32a713c3db4228b9681075edea0eebee53171649cd3"
    "7e0cf1d0f1c2c53a878f2f317c509ac145d46a71bfab8893de66eabf5da65f0af11089513d1ff819c449f5cb076cb207fb31603fc689b06bc87321cf556de7ff",
    "0718ad6716", "" };

  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(data, 71536U, &nameCaptureInfo);
  return nameCaptureInfo;
}

/* End of code generation (_coder_myNeuralNetworkFunction_info.c) */
