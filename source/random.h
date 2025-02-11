#ifndef SNAIL_JUMPY_RANDOM_H
#define SNAIL_JUMPY_RANDOM_H

global_constant u32 RANDOM_NUMBER_TABLE[1024] = {
    0xa984560e, 0x473e5245, 0x322585cc, 0x9f4b5ec3,
    0x1b3418a9, 0x8561cc2b, 0xf7e0a228, 0xccc5950f,
    0x32fd9f5f, 0xf2437b8e, 0xdaeeec08, 0xe235e295,
    0x9178cb8d, 0x58932e95, 0x0307ca14, 0xe785cd1e,
    0xdd3edcd4, 0x3c22916d, 0x5e0d9f49, 0x87a88d31,
    0xbebb6259, 0x955d5e0a, 0x587815ce, 0x0bb8a020,
    0x8d89536a, 0xa167176c, 0x6f9d1d1f, 0x4491a66d,
    0x9e8127eb, 0x844da378, 0x38e53518, 0xb66361e1,
    0xe0a5bdde, 0x41feb828, 0xb827d8fb, 0xb9f7c21d,
    0x5ebe8c22, 0x9454222c, 0xe95bc3a2, 0x9c3f7e5a,
    0xde7b5305, 0xd21678c6, 0x98c510a6, 0x1c5fd524,
    0xa9dbc47d, 0xdcdda330, 0xd0d5d633, 0x2538b368,
    0xe182a604, 0x2f6f7253, 0xfdad68d7, 0xa247cd70,
    0xb2802981, 0xdb111d43, 0xb7b223d4, 0x1742d7b5,
    0xafad2613, 0xf4ac88a4, 0x9f6d7c32, 0x860b454c,
    0x96fb8c00, 0xfd76bbf1, 0xe2cb13c3, 0xab6b7cad,
    0xe8344b85, 0x48c0d538, 0xad4d55c6, 0xece63d30,
    0x9a9b1f95, 0xbed4f4c1, 0xc2dea00a, 0x636df691,
    0x49b9cdf3, 0x309199f9, 0x483dccec, 0x7cdcc77a,
    0x72bdc9bf, 0x998cb82b, 0xa8d63d13, 0x06bd6df7,
    0xdb371e2d, 0xae4deb3d, 0x330f9381, 0xa3b1fcc9,
    0xae60962c, 0x719d5741, 0xf86e85ae, 0xfd0ff68e,
    0xa5e4637f, 0x6beab9bb, 0x4cc15862, 0x62c7cf14,
    0x5dbb9216, 0x85cf133b, 0x43f0b927, 0x56b9cf25,
    0xfc6ac84e, 0x048751d2, 0x3b636990, 0xf6635694,
    0xd35d39f4, 0x2acb79e4, 0x3ee59bd4, 0x81de49a5,
    0xdcc32d6d, 0xee33625b, 0x6b44ae49, 0x2733dc6d,
    0x601ac840, 0x56551b50, 0xa7a44ddf, 0x4d4aebe4,
    0x2674dcd3, 0x681dd6cd, 0x2cfa80bc, 0x9cb5ffdc,
    0x10a457d1, 0x8154cd83, 0xadebeefd, 0x2c68c2e1,
    0x7e6edab4, 0xb73991ef, 0xd0cdb68d, 0xb33bf22f,
    0xb6a8b83e, 0xef33e4d1, 0x99544048, 0xf2555fb9,
    0xfc001d33, 0xbabc8789, 0xc5206cb2, 0xf203947e,
    0x7108a0fb, 0x1036dcf5, 0x6d84efa7, 0x25f4e54d,
    0x572fb113, 0x83fe286d, 0x6311d50f, 0xce2a1f89,
    0x9bb1e689, 0xb154cf27, 0x2ee52783, 0xf4be5132,
    0x2f7f2ff3, 0x4645e079, 0x95afa414, 0xee25095a,
    0x0104ecba, 0x01655633, 0x3748f212, 0x337f8276,
    0xd25b385d, 0x45cb0060, 0xd2c6cba6, 0xc08c55a7,
    0xefd4d307, 0xa11526ac, 0x383f60a3, 0xa67e3376,
    0xfdca2cdf, 0x28e79832, 0x857e630f, 0x3ed96dfa,
    0xb8a2fc74, 0xb052213a, 0x3650e9f4, 0xce56ed20,
    0xcdf06767, 0x2213c0f0, 0x90dd86e4, 0x1850e8a4,
    0xa22a9bc7, 0xd39742da, 0xf9455f93, 0xc63e0e48,
    0xb477fb6c, 0xa68d3d1a, 0x41e94528, 0x7797aabc,
    0x7c312072, 0xa95d6d53, 0x25a47fcf, 0xf9fbdd9c,
    0x8dff9216, 0xce6d516d, 0x1c3fb77f, 0xfa4c1c12,
    0xcf7a0bf9, 0x971330f4, 0xd57e53af, 0x7adc58f3,
    0xebcd198f, 0xfd0700b9, 0xc7d2c5b0, 0x13f83c88,
    0x778f8489, 0xe7bd4aa6, 0xa86c7c19, 0x6f88eb75,
    0xb140d5bf, 0x8008bc4d, 0x9dd53ef3, 0x7959f2ff,
    0xc2361dc8, 0x33f19d3e, 0x3d858266, 0x838986e5,
    0x51ac323e, 0x084792a7, 0xeaa4d4d9, 0x169afe2f,
    0xd5d670a9, 0x9f6a7cde, 0xfd1248df, 0xd748a7f6,
    0x7514b6d4, 0xb7497cc9, 0x453afc86, 0xe6c68130,
    0xb6c99529, 0x404a78eb, 0x961edab1, 0x982549ef,
    0x86f1b79b, 0xbd9808aa, 0x0954f5d0, 0xadf48879,
    0x25516d34, 0x3eec830a, 0x79571e63, 0xbaa6b921,
    0x9397ef0d, 0x1c9a0b32, 0x72feca6a, 0xddf794f5,
    0x3c94fb77, 0x172f52a6, 0x6f890acf, 0xa27fbcad,
    0xd352a7f0, 0xcb52c44b, 0xba98ceb9, 0xb5fc644c,
    0xa491a9f8, 0x855d1b2a, 0xd9fc71cf, 0x1dd5cb04,
    0x8b282e4e, 0xb12f2719, 0xbfcffff4, 0xfb9e3247,
    0x1257f1d5, 0x1033b61b, 0x81ff047b, 0xbbe466c4,
    0xec1e2043, 0x4f87f7fb, 0x4bc5ac52, 0x6e3c6d77,
    0x418215cb, 0x8e4aba77, 0xab26e4bd, 0xfe900883,
    0x98b5b00f, 0x8f22666c, 0x9d0ab060, 0x7d75c37e,
    0xd3697d0d, 0x2bf4f205, 0xdd82f6a6, 0x7ad9b69b,
    0xe420f7e6, 0xf080e12c, 0x42cf42ac, 0x42de3797,
    0xfb7376e2, 0xd89a635c, 0xfe3c07fb, 0x9428de61,
    0x7c450b1f, 0xa7b7454b, 0xae807c59, 0x933d7138,
    0xe3aa5a7e, 0xc2de08bd, 0x0c50a113, 0x47f9296a,
    0x2377bc56, 0xd21a7125, 0x52d3adad, 0x56e20e7a,
    0xa2f1c8b9, 0xac391664, 0x46ba975a, 0xfdb027de,
    0xc569fcc8, 0x14e275d0, 0x064190af, 0xef005403,
    0x95ed8704, 0x93ab539c, 0x4dbbbab6, 0x470dd16e,
    0xdff50311, 0xf92b4abb, 0x28060486, 0x39f31932,
    0xe5a06e07, 0x91f344de, 0x4cc5ff49, 0x0c4ce64d,
    0x45cfac08, 0x99767833, 0xab276252, 0xb571a7e2,
    0x64f903c1, 0x19891464, 0x8b6d11f0, 0x7d55f56a,
    0x0b9d52e7, 0x87b420eb, 0x14bbb43e, 0xb4b4a23b,
    0x452c4eee, 0x794f01f3, 0x57b7ab56, 0x823d4d73,
    0x5d817371, 0xab3eb652, 0xf338549c, 0x2a0c6d89,
    0x2c7b3af7, 0xedfad060, 0xc85f70cb, 0xbb30cdbd,
    0xc5d7fccb, 0x825e7ae1, 0x180093f4, 0x29261774,
    0x6a49755d, 0x372161c2, 0xb2a052a3, 0xfc227915,
    0x4f321583, 0x0d6b0ed5, 0x35398a99, 0x0bf07c4c,
    0x3f1e2fb2, 0x6ec066f0, 0x9a437ce8, 0x00c88705,
    0x4a4f58d9, 0x73104512, 0xeb6da885, 0x745bb4d8,
    0x29a05685, 0x7e9c9f03, 0x5e451b20, 0x735d850c,
    0xa6a17eda, 0x2e67c0e1, 0x1d6a6606, 0x61b30ae4,
    0x9752a095, 0xe6634bec, 0xdd77fe2f, 0x5aa3e304,
    0x4c38c217, 0x02d70a83, 0xe7262e68, 0xe3e43923,
    0x37c408ce, 0xc362e429, 0xaa13fc6b, 0x9c846f63,
    0x5cfe8aec, 0xff6ccd69, 0x5c639bd8, 0x33f1d5bd,
    0x72ed7e17, 0x91009353, 0x5cac567f, 0xaaa76fed,
    0x5ee2bf9f, 0x77beeffc, 0x5c64af2e, 0x825478fa,
    0x5079a93a, 0xb7b7a08a, 0xf588c0af, 0xae1cbbcc,
    0xb0cd48cf, 0xd584918c, 0x6f65cfd1, 0x13a8d528,
    0xe0893d61, 0xaa5ac9b8, 0xddf82336, 0xe5d007fc,
    0x9d2ab198, 0x320d5776, 0x3e7aee09, 0x5e944caf,
    0x4e7baf09, 0x0ff0b357, 0x02465aa8, 0xc82cba20,
    0x1b9144a7, 0xb09d19a7, 0x475050c8, 0x0d870aa5,
    0x74f58a43, 0x729e21c2, 0x0a4c2dda, 0xa938204a,
    0xca1a19b7, 0x60f27283, 0x7cd340ea, 0xf576d92a,
    0x35dfc65b, 0x1e88e39e, 0x70f396cc, 0x3f2e4263,
    0xd253db8b, 0xeda0b23e, 0xa017246c, 0x6ee11ca7,
    0xe08f35b6, 0x771907c9, 0x5954f195, 0x8ad1e313,
    0x620e0553, 0x403930ae, 0x5e773b3f, 0x553667c6,
    0x6bca0702, 0x0ac63984, 0x30225628, 0x64ff8528,
    0x18690da0, 0xc96d1080, 0x95c46868, 0xbc87effa,
    0xb7c1fdae, 0xac4e817b, 0x71994b83, 0x876da61e,
    0xc02c2ca3, 0x7d8ca304, 0x5073a1cf, 0xaead8a0f,
    0x8d230ea7, 0xad799727, 0x473b18a0, 0xa2237b1f,
    0xca6382a1, 0x614eb4f2, 0xd72a3fc4, 0x18cc2adf,
    0xb4f296c0, 0x26356349, 0x399ecb27, 0x9792a9c5,
    0xee4073b2, 0xb74f3bff, 0x45d9db4d, 0x28a5b501,
    0xd3843824, 0xb6296f3e, 0x20f6b787, 0x9476ac49,
    0xc6e8216a, 0x7a201660, 0xa8fb1f05, 0xf25b5b8c,
    0xf3a957a7, 0x0d8e9598, 0x748e0ffc, 0x189347ca,
    0x54b2b16d, 0x7ffc67e0, 0x1893cf0f, 0xb4aeddde,
    0x0e3e347d, 0xb8ae3006, 0xaa2f6124, 0x0d409ddd,
    0x3996865a, 0xc2c06739, 0x9e4d60a5, 0x5742a2b3,
    0xcbda0dd0, 0x6fa01887, 0x610e7761, 0x9ba35a88,
    0x9f2ed135, 0x2269800b, 0xf6e4f090, 0xb0bcd2d0,
    0xf79cc2f9, 0x6ea5f7a0, 0x813c1b9f, 0x73921eea,
    0x1a42d114, 0x4250ed2d, 0x503fae1c, 0xb11bd217,
    0x0d5f441f, 0x62b2f13f, 0x4f84d572, 0xc83384da,
    0x71b8c54f, 0x222bcb96, 0x25f62d68, 0xbb970131,
    0x0921aa49, 0x43218001, 0x1f8167b9, 0x5dd03b87,
    0xd8b0d7bb, 0xb5a9affc, 0x8047570c, 0x43cca4ee,
    0x7d87bc35, 0xfcf856d6, 0x9c5c9b2e, 0x51d0b867,
    0x99fd868f, 0xe6c5e14d, 0x20213c0f, 0x16ed5a72,
    0xe64dcdea, 0x2eea9453, 0xab561078, 0x253edca6,
    0x2f75da5e, 0x5710ed87, 0x3a9f65c4, 0xd839ddb4,
    0x0a8fa1a5, 0x59115cc9, 0x491f574b, 0xb57128de,
    0x09ccc464, 0x427becdb, 0xcc2587df, 0x0fd56fb3,
    0xa6f108f4, 0x4e4cde1e, 0x34a23011, 0x139ca91e,
    0x63d10c91, 0xaaf8e6e2, 0x6edb2085, 0x19b40f20,
    0x1dd9703c, 0x316bb618, 0xde6f0ae6, 0x9aa32422,
    0xfeb04bb1, 0x605f73e8, 0x986de1e5, 0xdddfe62a,
    0x46a82300, 0x1cd22620, 0xdab5598c, 0x8aeef2cc,
    0xd5326002, 0x6d0e20dd, 0x654648eb, 0x87addef2,
    0xecc0b180, 0x1ae91803, 0xff8c8e71, 0x3978813b,
    0xeed3fa07, 0xbabd602a, 0xe11ee3d8, 0x8a447286,
    0xe81d1b05, 0xf8f65889, 0x7165ea23, 0xa792bf96,
    0xbff32ced, 0x161a933a, 0xc217f472, 0x538cf4c3,
    0x9e1e4c0f, 0xc5863939, 0x39e4c0ac, 0x5e17eac2,
    0xa79a0724, 0x2f96acd9, 0xf310eea4, 0xe4f6a448,
    0x65a87b81, 0xe967ff3e, 0xe190d53b, 0x143c0702,
    0x9992c669, 0x9d4e860c, 0xd7b9e443, 0xf4f45711,
    0x016c5632, 0xf6a173be, 0xa96291be, 0x96efbce4,
    0x04cbcb06, 0xbd2bd6d3, 0xb250a90b, 0x106a099c,
    0xcc3ae22d, 0xdc7b8987, 0x0ed23b65, 0x87dc0bfe,
    0x6a04a139, 0xaddf9794, 0x38ba400c, 0xa166f63c,
    0x57f3fc2d, 0xbfe8ea3d, 0x617d637f, 0xbadad833,
    0xf1597a9d, 0x8d53012e, 0x3bee4105, 0xaf34e9bb,
    0x2e8fcb32, 0xa70872e7, 0xc722df32, 0x44a1dbf7,
    0x6d263492, 0x17590a55, 0xb844f1f7, 0x5e9fd0bb,
    0x6c82a4f6, 0x45454ad2, 0x12ac3b65, 0x55b84810,
    0xb47dc2da, 0x920a65ad, 0x3674db41, 0xc952b001,
    0x998d56f0, 0x17f5de26, 0xb100d5b6, 0xfdf1e0c8,
    0x69972bc4, 0x09e152db, 0x0d71c14e, 0x1911c9cc,
    0xb7edbd85, 0xad54af07, 0x40f02ca3, 0xc7797e1d,
    0xd614ac13, 0x88bdc2b2, 0x7ce3ebce, 0xc9d13290,
    0xb160a761, 0xe928f3a4, 0x9b4ab20b, 0x0edef835,
    0xc6969174, 0x58ab67b8, 0x9c277c81, 0x590c2a5c,
    0x06b7f4e2, 0x1734edd1, 0x4aee5040, 0x59188607,
    0x361c29a2, 0x3386e94f, 0x92b56dc6, 0x7022c38f,
    0xbc070b7e, 0x4ac62354, 0x613a1d26, 0xfe275735,
    0x1c5001a6, 0xabe8532f, 0xb459f954, 0x6e5bda4c,
    0xeec4e47a, 0x93b42b4e, 0xb8d19e81, 0x0df6f799,
    0x2ffae04f, 0x86976855, 0xfb65cd7f, 0xf04e461d,
    0x75ffd4db, 0xfc5e1962, 0x47f6d351, 0xe2768a0e,
    0x196f9447, 0x4fcdc5ca, 0xdcd7a91c, 0x72473a9f,
    0xf9b6f2aa, 0x71b3265a, 0x5f2ca2bc, 0x25caa29b,
    0x30b47f82, 0x62455a5e, 0xbaa5e0f4, 0x2578d20e,
    0xa71e0d9a, 0xfb37ed99, 0xe4d12e66, 0x47782627,
    0x7e2cb364, 0x2a85911c, 0x56a6f2d7, 0xa21c50cc,
    0x7687762d, 0x3f3c2f49, 0x62ce0c6c, 0x857d4f9c,
    0x628f986c, 0xb5c9dae5, 0xf486f6f6, 0x7088843b,
    0xaeacb195, 0x9c3e1d8a, 0x267df7bd, 0x13ed8b72,
    0xe8a0d3ac, 0xcba719e6, 0x72a7c5b6, 0xc6a43c54,
    0xf26867af, 0x42e0d707, 0x677c7d85, 0x57bfca49,
    0xf37dc100, 0xf9dc356b, 0x3720cd7b, 0x16c9e84a,
    0x0f266f7e, 0xa126ff1d, 0x341f7b60, 0x37559a9c,
    0xd37bca25, 0x21eede6e, 0x0781615c, 0xe1067dbf,
    0x48077bd1, 0x96b48c73, 0xb7743f7a, 0xac2617c9,
    0xc971c5d8, 0x206a4d9e, 0xc4020077, 0x577eea39,
    0x048f2bc3, 0x03fbb5de, 0x57d65951, 0xf8aef1a0,
    0xfb68b8f2, 0x25095804, 0xdd73e2ff, 0x95d0503b,
    0x853448bb, 0x1282978b, 0x29d9f14f, 0x86337da7,
    0xde9d3bdd, 0x13a12770, 0x02db6a57, 0x99668654,
    0x64aa55c7, 0xb1f84e34, 0x18e4e4ac, 0x8a825f7e,
    0xd559ceba, 0x1dbbd40c, 0x818711ca, 0x2428dc9b,
    0xb04b13ad, 0xa14ef49c, 0x7718afa9, 0xa5c06ab6,
    0xc5f3ddad, 0xffac1967, 0x073dbf02, 0x3c77e0c2,
    0xb1f0f5e6, 0x8f1deaaf, 0x23a1e4b6, 0x846ea508,
    0xd73a0817, 0xffbee018, 0xe8369bc2, 0xf5b94d37,
    0x18bbd722, 0x63adc7f1, 0x2aa96892, 0x96b5a79d,
    0x5e2a0299, 0x1affb9ff, 0xed75d1cd, 0xa42ab698,
    0x78234701, 0x3cc5c827, 0xafe53573, 0xab618d71,
    0xc981a713, 0xf6482d93, 0x818e006c, 0x0bf0f7c2,
    0x9d5bc246, 0xa0f10071, 0x87909089, 0x40ca6d0a,
    0xc4d46b7c, 0x796d1107, 0xed66601d, 0xefe6b016,
    0x724c898e, 0xda2816c5, 0x80b06a6b, 0xb27f6cce,
    0xef0b8755, 0x5154d16e, 0x164068bc, 0x3f75dd6f,
    0x700ce3b8, 0x51db3279, 0x6e55b5ff, 0x8754850f,
    0x5402cbd2, 0xe21b13c2, 0x9c0958da, 0xaf90ee61,
    0x3ebc6a03, 0xe9bb7295, 0xad9189e5, 0x12ce272e,
    0x895a5d89, 0xf439d29a, 0x58617ed5, 0x9bea07ed,
    0xd62a9839, 0x7985ee4b, 0xbbe2c0e5, 0x0a78e91c,
    0x16e5bce6, 0xcc2cb5bd, 0x09497528, 0xe98ceeec,
    0x0c1fec1c, 0x230f88d8, 0xa3342c3b, 0x9f51da1f,
    0x0d491946, 0x0c2a7350, 0xb4363cfa, 0x375aa23b,
    0xd276aaea, 0xb4345c14, 0x745dc023, 0xcbee00b9,
    0xc0088858, 0x83416823, 0x2c522409, 0xcdc657b3,
    0x99d51c63, 0xf67e6dce, 0x539f5b8b, 0x65394d21,
    0xae6d5269, 0xc5c72cdb, 0xe0b64ea2, 0xd7a11e93,
    0xe8988108, 0xb2cbdddb, 0x9f534a10, 0x7337e576,
    0x4efb2279, 0x627527b8, 0x40a830c6, 0x6c7510d9,
    0x4b3662c0, 0xea846015, 0x85818988, 0xa4eb46ed,
    0xcaadbfb2, 0x0c55eb21, 0xd6c83c72, 0xad0b99a2,
    0xec55b43c, 0x8d54f718, 0xe5909ec7, 0x321e1274,
    0x73d296e5, 0x43a672d0, 0x6874cf58, 0xc16464bc,
    0x904ac1a5, 0x13dcde4a, 0xe8af4408, 0x45fb0d76,
    0xcd0a561c, 0x3ba64f90, 0x6d9e3231, 0xcd546dfa,
    0x428104c3, 0x93e8c5d7, 0xb8dad409, 0x3086ca78,
    0x83b52be2, 0x9696d843, 0x43ecee3b, 0xa502d8b4,
    0x3acd46c1, 0x947265d4, 0x417ff345, 0x6701f751,
    0x811e3270, 0xed7aa489, 0x235dfc4d, 0xc50e3d39,
    0x570d4ed4, 0xa3f187d8, 0x7e1917d9, 0xa0d8e7a6,
    0x851ade22, 0x29a5a5a2, 0xd0068fd7, 0xbb4a3861,
    0x64faf344, 0xbd944eae, 0xc5ad3091, 0xdc8bb45c,
    0x5375f5ac, 0x6e88589a, 0xe8b10ed7, 0x82a9a8e1,
    0xc7d6e13b, 0xdff39ed2, 0xbd9b59dc, 0x3756450d,
    0x83c8da4b, 0x5ec36f14, 0x81a94570, 0x7219a5f9,
    0x956bbe78, 0xe583d4a9, 0xf16e096b, 0xe86a8b3d,
    0xb88ad689, 0x55f6949f, 0x43f047fb, 0x4f0e17c6,
    0x90d2606e, 0x3236dcb4, 0x036ddf5a, 0x480c1e51,
    0x7d23344c, 0x6fa5cb24, 0x008a0cbb, 0xb66bcf29,
    0xdf661d70, 0xe67c2ea6, 0x8fc73503, 0x25b94db8,
    0x87833411, 0xd97b9368, 0x4bb5e0bb, 0x1143b9bc,
    0x8789681c, 0x8951e899, 0x633f5684, 0xcb563659,
    0x20e6b93a, 0x0fd1556c, 0xda517bc1, 0xe6ea7969,
    0xac604ea9, 0xa0b15e4a, 0x97f3bb74, 0x4000bc90,
    0x89423e07, 0x61bc6808, 0x5ed6f0fb, 0x69f071ec,
    0x2d321dcf, 0x8525ff89, 0x1918445e, 0x10ecb6b2,
    0x05d1ec7d, 0xd611b18b, 0x8f83b089, 0x0506acb9,
    0x574b2dc0, 0x795c6b09, 0xc86a4a52, 0x3f7ff6e4,
    0x1fd7f668, 0x080b24d1, 0x8f1db148, 0xa46ad997,
    0xbd035d69, 0x0adaa210, 0x9caeeec8, 0xc530afed,
    0x9b5d2112, 0xf944eb0f, 0x62282d88, 0x10de7e5b,
    0xbb05c4b4, 0xf7ce60bc, 0x378cd2b4, 0x1f09dee5,
    0x14c0e02f, 0xf2f19218, 0x33be07a9, 0xc6ea671f,
    0x14711603, 0xdcb81c12, 0xa7820b16, 0xfaefcc56
};

#endif //SNAIL_JUMPY_RANDOM_H
