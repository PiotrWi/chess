#include "bitboardslookups.hpp"

#ifdef USE_PEXT

BishopPextBB bishopBb = {};
RockPextBB rockBb = {};

BishopPextBB::BishopPextBB()
{
    lookup[0].relevantBlockers = 18049651735527936ull;
    lookup[1].relevantBlockers = 70506452091904ull;
    lookup[2].relevantBlockers = 275415828992ull;
    lookup[3].relevantBlockers = 1075975168ull;
    lookup[4].relevantBlockers = 38021120ull;
    lookup[5].relevantBlockers = 8657588224ull;
    lookup[6].relevantBlockers = 2216338399232ull;
    lookup[7].relevantBlockers = 567382630219776ull;
    lookup[8].relevantBlockers = 9024825867763712ull;
    lookup[9].relevantBlockers = 18049651735527424ull;
    lookup[10].relevantBlockers = 70506452221952ull;
    lookup[11].relevantBlockers = 275449643008ull;
    lookup[12].relevantBlockers = 9733406720ull;
    lookup[13].relevantBlockers = 2216342585344ull;
    lookup[14].relevantBlockers = 567382630203392ull;
    lookup[15].relevantBlockers = 1134765260406784ull;
    lookup[16].relevantBlockers = 4512412933816832ull;
    lookup[17].relevantBlockers = 9024825867633664ull;
    lookup[18].relevantBlockers = 18049651768822272ull;
    lookup[19].relevantBlockers = 70515108615168ull;
    lookup[20].relevantBlockers = 2491752130560ull;
    lookup[21].relevantBlockers = 567383701868544ull;
    lookup[22].relevantBlockers = 1134765256220672ull;
    lookup[23].relevantBlockers = 2269530512441344ull;
    lookup[24].relevantBlockers = 2256206450263040ull;
    lookup[25].relevantBlockers = 4512412900526080ull;
    lookup[26].relevantBlockers = 9024834391117824ull;
    lookup[27].relevantBlockers = 18051867805491712ull;
    lookup[28].relevantBlockers = 637888545440768ull;
    lookup[29].relevantBlockers = 1135039602493440ull;
    lookup[30].relevantBlockers = 2269529440784384ull;
    lookup[31].relevantBlockers = 4539058881568768ull;
    lookup[32].relevantBlockers = 1128098963916800ull;
    lookup[33].relevantBlockers = 2256197927833600ull;
    lookup[34].relevantBlockers = 4514594912477184ull;
    lookup[35].relevantBlockers = 9592139778506752ull;
    lookup[36].relevantBlockers = 19184279556981248ull;
    lookup[37].relevantBlockers = 2339762086609920ull;
    lookup[38].relevantBlockers = 4538784537380864ull;
    lookup[39].relevantBlockers = 9077569074761728ull;
    lookup[40].relevantBlockers = 562958610993152ull;
    lookup[41].relevantBlockers = 1125917221986304ull;
    lookup[42].relevantBlockers = 2814792987328512ull;
    lookup[43].relevantBlockers = 5629586008178688ull;
    lookup[44].relevantBlockers = 11259172008099840ull;
    lookup[45].relevantBlockers = 22518341868716544ull;
    lookup[46].relevantBlockers = 9007336962655232ull;
    lookup[47].relevantBlockers = 18014673925310464ull;
    lookup[48].relevantBlockers = 2216338399232ull;
    lookup[49].relevantBlockers = 4432676798464ull;
    lookup[50].relevantBlockers = 11064376819712ull;
    lookup[51].relevantBlockers = 22137335185408ull;
    lookup[52].relevantBlockers = 44272556441600ull;
    lookup[53].relevantBlockers = 87995357200384ull;
    lookup[54].relevantBlockers = 35253226045952ull;
    lookup[55].relevantBlockers = 70506452091904ull;
    lookup[56].relevantBlockers = 567382630219776ull;
    lookup[57].relevantBlockers = 1134765260406784ull;
    lookup[58].relevantBlockers = 2832480465846272ull;
    lookup[59].relevantBlockers = 5667157807464448ull;
    lookup[60].relevantBlockers = 11333774449049600ull;
    lookup[61].relevantBlockers = 22526811443298304ull;
    lookup[62].relevantBlockers = 9024825867763712ull;
    lookup[63].relevantBlockers = 18049651735527936ull;

    lookup[0].attacks = attacks;
    for (unsigned char i = 1u; i < 64; ++i)
    {
        lookup[i].attacks = lookup[i-1].attacks + (1ull << __builtin_popcountll(lookup[i-1].relevantBlockers));
    }

    for (unsigned char i = 0u; i < 64; ++i)
    {
        auto bitIndexes = extractSetBitIndexes(lookup[i].relevantBlockers);
        auto relevantBitsCount = bitIndexes.size();

        auto bitOptionsCount = (1ul << relevantBitsCount);

        for (auto j = 0u; j < bitOptionsCount; ++j)
        {
            auto blockers = 0ull;
            for (auto k = 0u; k< relevantBitsCount; ++k)
            {
                if ((1 << k) & j)
                {
                    blockers |= (1ull << bitIndexes[k]);
                }
            }
            auto currentAttacks = evaluateDiagonalAttacks(blockers, i);
            auto key = _pext_u64(blockers, lookup[i].relevantBlockers);
            lookup[i].attacks[key] = currentAttacks;
        }
    }
}

RockPextBB::RockPextBB()
{
    lookup[0].relevantBlockers = 282578800148862ull;
    lookup[1].relevantBlockers = 565157600297596ull;
    lookup[2].relevantBlockers = 1130315200595066ull;
    lookup[3].relevantBlockers = 2260630401190006ull;
    lookup[4].relevantBlockers = 4521260802379886ull;
    lookup[5].relevantBlockers = 9042521604759646ull;
    lookup[6].relevantBlockers = 18085043209519166ull;
    lookup[7].relevantBlockers = 36170086419038334ull;
    lookup[8].relevantBlockers = 282578800180736ull;
    lookup[9].relevantBlockers = 565157600328704ull;
    lookup[10].relevantBlockers = 1130315200625152ull;
    lookup[11].relevantBlockers = 2260630401218048ull;
    lookup[12].relevantBlockers = 4521260802403840ull;
    lookup[13].relevantBlockers = 9042521604775424ull;
    lookup[14].relevantBlockers = 18085043209518592ull;
    lookup[15].relevantBlockers = 36170086419037696ull;
    lookup[16].relevantBlockers = 282578808340736ull;
    lookup[17].relevantBlockers = 565157608292864ull;
    lookup[18].relevantBlockers = 1130315208328192ull;
    lookup[19].relevantBlockers = 2260630408398848ull;
    lookup[20].relevantBlockers = 4521260808540160ull;
    lookup[21].relevantBlockers = 9042521608822784ull;
    lookup[22].relevantBlockers = 18085043209388032ull;
    lookup[23].relevantBlockers = 36170086418907136ull;
    lookup[24].relevantBlockers = 282580897300736ull;
    lookup[25].relevantBlockers = 565159647117824ull;
    lookup[26].relevantBlockers = 1130317180306432ull;
    lookup[27].relevantBlockers = 2260632246683648ull;
    lookup[28].relevantBlockers = 4521262379438080ull;
    lookup[29].relevantBlockers = 9042522644946944ull;
    lookup[30].relevantBlockers = 18085043175964672ull;
    lookup[31].relevantBlockers = 36170086385483776ull;
    lookup[32].relevantBlockers = 283115671060736ull;
    lookup[33].relevantBlockers = 565681586307584ull;
    lookup[34].relevantBlockers = 1130822006735872ull;
    lookup[35].relevantBlockers = 2261102847592448ull;
    lookup[36].relevantBlockers = 4521664529305600ull;
    lookup[37].relevantBlockers = 9042787892731904ull;
    lookup[38].relevantBlockers = 18085034619584512ull;
    lookup[39].relevantBlockers = 36170077829103616ull;
    lookup[40].relevantBlockers = 420017753620736ull;
    lookup[41].relevantBlockers = 699298018886144ull;
    lookup[42].relevantBlockers = 1260057572672512ull;
    lookup[43].relevantBlockers = 2381576680245248ull;
    lookup[44].relevantBlockers = 4624614895390720ull;
    lookup[45].relevantBlockers = 9110691325681664ull;
    lookup[46].relevantBlockers = 18082844186263552ull;
    lookup[47].relevantBlockers = 36167887395782656ull;
    lookup[48].relevantBlockers = 35466950888980736ull;
    lookup[49].relevantBlockers = 34905104758997504ull;
    lookup[50].relevantBlockers = 34344362452452352ull;
    lookup[51].relevantBlockers = 33222877839362048ull;
    lookup[52].relevantBlockers = 30979908613181440ull;
    lookup[53].relevantBlockers = 26493970160820224ull;
    lookup[54].relevantBlockers = 17522093256097792ull;
    lookup[55].relevantBlockers = 35607136465616896ull;
    lookup[56].relevantBlockers = 9079539427579068672ull;
    lookup[57].relevantBlockers = 8935706818303361536ull;
    lookup[58].relevantBlockers = 8792156787827803136ull;
    lookup[59].relevantBlockers = 8505056726876686336ull;
    lookup[60].relevantBlockers = 7930856604974452736ull;
    lookup[61].relevantBlockers = 6782456361169985536ull;
    lookup[62].relevantBlockers = 4485655873561051136ull;
    lookup[63].relevantBlockers = 9115426935197958144ull;

    lookup[0].attacks = attacks;
    for (unsigned char i = 1u; i < 64; ++i)
    {
        lookup[i].attacks = lookup[i-1].attacks + (1ull << __builtin_popcountll(lookup[i-1].relevantBlockers));
    }

    for (unsigned char i = 0u; i < 64; ++i)
    {
        auto bitIndexes = extractSetBitIndexes(lookup[i].relevantBlockers);
        auto relevantBitsCount = bitIndexes.size();

        auto bitOptionsCount = (1ul << relevantBitsCount);

        for (auto j = 0u; j < bitOptionsCount; ++j)
        {
            auto blockers = 0ull;
            for (auto k = 0u; k< relevantBitsCount; ++k)
            {
                if ((1 << k) & j)
                {
                    blockers |= (1ull << bitIndexes[k]);
                }
            }
            auto currentAttacks = evaluateLineAttacks(blockers, i);
            auto key = _pext_u64(blockers, lookup[i].relevantBlockers);
            lookup[i].attacks[key] = currentAttacks;
        }
    }
}

#endif
