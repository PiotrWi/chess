#include "bitboardslookups.hpp"

#include <iostream>

BishopMagicBitBoards bishopMagicBb = {};
RockMagicBitBoards rockMagicBb = {};

std::vector<unsigned> extractSetBitIndexes(uint64_t in)
{
    std::vector<unsigned> out;
    for (unsigned char i = 0u; i < 64; ++i)
    {
        if ((1ull << i) & in)
        {
            out.push_back(i);
        }
    }
    return out;
}

uint64_t evaluateLineAttacks(uint64_t blockers, uint piecePosition)
{
    const auto& lookup = bitBoardLookup[piecePosition];
    uint64_t attacks = 0ull;

    {
        auto topPieces = lookup.topRay & blockers;
        if (topPieces)
        {
            auto topBlocker = __builtin_ffsll(topPieces) - 1;
            attacks |=(lookup.topRay ^ (1ull << topBlocker) ^ bitBoardLookup[topBlocker].topRay);
            attacks |= (1ull << topBlocker);
        }
        else
        {
            attacks |= lookup.topRay;
        }
    }
    {
        auto rightPieces = lookup.rightRay & blockers;
        if (rightPieces)
        {
            auto rightBlocker = __builtin_ffsll(rightPieces) - 1;
            attacks |=(lookup.rightRay ^ (1ull << rightBlocker) ^ bitBoardLookup[rightBlocker].rightRay);
            attacks |= (1ull << rightBlocker);
        }
        else
        {
            attacks |= lookup.rightRay;
        }
    }
    {
        auto leftPieces = lookup.leftRay & blockers;
        if (leftPieces)
        {
            auto leftBlocker = 63 - __builtin_clzll(leftPieces);
            attacks |=(lookup.leftRay ^ (1ull << leftBlocker) ^ bitBoardLookup[leftBlocker].leftRay);
            attacks |= (1ull << leftBlocker);
        }
        else
        {
            attacks |= lookup.leftRay;
        }
    }
    {
        auto bottomPieces = lookup.bottomRay & blockers;
        if (bottomPieces)
        {
            auto bottomBlocker = 63 - __builtin_clzll(bottomPieces);
            attacks |=(lookup.bottomRay ^ (1ull << bottomBlocker) ^ bitBoardLookup[bottomBlocker].bottomRay);
            attacks |= (1ull << bottomBlocker);
        }
        else
        {
            attacks |= lookup.bottomRay;
        }
    }
    return attacks;
}


uint64_t evaluateDiagonalAttacks(uint64_t blockers, uint piecePosition)
{
    const auto& lookup = bitBoardLookup[piecePosition];
    uint64_t attacks = 0ull;

    {
        auto topLeftPieces = lookup.topLeft & blockers;
        if (topLeftPieces)
        {
            auto topLeftBlocker = __builtin_ffsll(topLeftPieces) - 1;
            attacks |=(lookup.topLeft ^ (1ull << topLeftBlocker) ^ bitBoardLookup[topLeftBlocker].topLeft);
            attacks |= (1ull << topLeftBlocker);
        }
        else
        {
            attacks |= lookup.topLeft;
        }
    }
    {
        auto topRightPieces = lookup.topRight & blockers;
        if (topRightPieces)
        {
            auto topRightBlocker = __builtin_ffsll(topRightPieces) - 1;
            attacks |=(lookup.topRight ^ (1ull << topRightBlocker) ^ bitBoardLookup[topRightBlocker].topRight);
            attacks |= (1ull << topRightBlocker);
        }
        else
        {
            attacks |= lookup.topRight;
        }
    }
    {
        auto bottomLeftPieces = lookup.bottomLeft & blockers;
        if (bottomLeftPieces)
        {
            auto bottomLeftBlocker = 63 - __builtin_clzll(bottomLeftPieces);
            attacks |=(lookup.bottomLeft ^ (1ull << bottomLeftBlocker) ^ bitBoardLookup[bottomLeftBlocker].bottomLeft);
            attacks |= (1ull << bottomLeftBlocker);
        }
        else
        {
            attacks |= lookup.bottomLeft;
        }
    }
    {
        auto bottomRightPieces = lookup.bottomRight & blockers;
        if (bottomRightPieces)
        {
            auto bottomRightBlocker = 63 - __builtin_clzll(bottomRightPieces);
            attacks |=(lookup.bottomRight ^ (1ull << bottomRightBlocker) ^ bitBoardLookup[bottomRightBlocker].bottomRight);
            attacks |= (1ull << bottomRightBlocker);
        }
        else
        {
            attacks |= lookup.bottomRight;
        }
    }
    return attacks;
}

BishopMagicBitBoards::BishopMagicBitBoards()
{
lookup[0].relevantBlockers = 18049651735527936ull;
lookup[0].magicMultiplier = 9007339386735104ull;
lookup[1].relevantBlockers = 70506452091904ull;
lookup[1].magicMultiplier = 708637395076608ull;
lookup[2].relevantBlockers = 275415828992ull;
lookup[2].magicMultiplier = 4631393682058649744ull;
lookup[3].relevantBlockers = 1075975168ull;
lookup[3].magicMultiplier = 144401061669502976ull;
lookup[4].relevantBlockers = 38021120ull;
lookup[4].magicMultiplier = 9266303705311871489ull;
lookup[5].relevantBlockers = 8657588224ull;
lookup[5].magicMultiplier = 6057377405276783680ull;
lookup[6].relevantBlockers = 2216338399232ull;
lookup[6].magicMultiplier = 1874624599027753088ull;
lookup[7].relevantBlockers = 567382630219776ull;
lookup[7].magicMultiplier = 46457061638440ull;
lookup[8].relevantBlockers = 9024825867763712ull;
lookup[8].magicMultiplier = 17936455041552ull;
lookup[9].relevantBlockers = 18049651735527424ull;
lookup[9].magicMultiplier = 4647824775334334466ull;
lookup[10].relevantBlockers = 70506452221952ull;
lookup[10].magicMultiplier = 45389752304896ull;
lookup[11].relevantBlockers = 275449643008ull;
lookup[11].magicMultiplier = 576461027265251328ull;
lookup[12].relevantBlockers = 9733406720ull;
lookup[12].magicMultiplier = 9594989512832843792ull;
lookup[13].relevantBlockers = 2216342585344ull;
lookup[13].magicMultiplier = 288240289204683266ull;
lookup[14].relevantBlockers = 567382630203392ull;
lookup[14].magicMultiplier = 595073440310895648ull;
lookup[15].relevantBlockers = 1134765260406784ull;
lookup[15].magicMultiplier = 4756126937092195536ull;
lookup[16].relevantBlockers = 4512412933816832ull;
lookup[16].magicMultiplier = 4688599090626830848ull;
lookup[17].relevantBlockers = 9024825867633664ull;
lookup[17].magicMultiplier = 38526973337669664ull;
lookup[18].relevantBlockers = 18049651768822272ull;
lookup[18].magicMultiplier = 1154339977694544913ull;
lookup[19].relevantBlockers = 70515108615168ull;
lookup[19].magicMultiplier = 2331550918213632ull;
lookup[20].relevantBlockers = 2491752130560ull;
lookup[20].magicMultiplier = 70437548331264ull;
lookup[21].relevantBlockers = 567383701868544ull;
lookup[21].magicMultiplier = 9024791585489996ull;
lookup[22].relevantBlockers = 1134765256220672ull;
lookup[22].magicMultiplier = 1301891563659264ull;
lookup[23].relevantBlockers = 2269530512441344ull;
lookup[23].magicMultiplier = 1729522994709039105ull;
lookup[24].relevantBlockers = 2256206450263040ull;
lookup[24].magicMultiplier = 285881632588864ull;
lookup[25].relevantBlockers = 4512412900526080ull;
lookup[25].magicMultiplier = 76704130205220873ull;
lookup[26].relevantBlockers = 9024834391117824ull;
lookup[26].magicMultiplier = 576482828569674004ull;
lookup[27].relevantBlockers = 18051867805491712ull;
lookup[27].magicMultiplier = 81636539406270728ull;
lookup[28].relevantBlockers = 637888545440768ull;
lookup[28].magicMultiplier = 4756526884185972736ull;
lookup[29].relevantBlockers = 1135039602493440ull;
lookup[29].magicMultiplier = 1155173441863688448ull;
lookup[30].relevantBlockers = 2269529440784384ull;
lookup[30].magicMultiplier = 72446821493937280ull;
lookup[31].relevantBlockers = 4539058881568768ull;
lookup[31].magicMultiplier = 1127027342053896ull;
lookup[32].relevantBlockers = 1128098963916800ull;
lookup[32].magicMultiplier = 378678685466656ull;
lookup[33].relevantBlockers = 2256197927833600ull;
lookup[33].magicMultiplier = 635025143945036161ull;
lookup[34].relevantBlockers = 4514594912477184ull;
lookup[34].magicMultiplier = 432845868940394512ull;
lookup[35].relevantBlockers = 9592139778506752ull;
lookup[35].magicMultiplier = 596735781096262144ull;
lookup[36].relevantBlockers = 19184279556981248ull;
lookup[36].magicMultiplier = 18018813802987776ull;
lookup[37].relevantBlockers = 2339762086609920ull;
lookup[37].magicMultiplier = 22590566206406720ull;
lookup[38].relevantBlockers = 4538784537380864ull;
lookup[38].magicMultiplier = 256706347125969924ull;
lookup[39].relevantBlockers = 9077569074761728ull;
lookup[39].magicMultiplier = 36192624285058080ull;
lookup[40].relevantBlockers = 562958610993152ull;
lookup[40].magicMultiplier = 73467734947528769ull;
lookup[41].relevantBlockers = 1125917221986304ull;
lookup[41].magicMultiplier = 2992939638726786ull;
lookup[42].relevantBlockers = 2814792987328512ull;
lookup[42].magicMultiplier = 2305884825082396944ull;
lookup[43].relevantBlockers = 5629586008178688ull;
lookup[43].magicMultiplier = 563224999100448ull;
lookup[44].relevantBlockers = 11259172008099840ull;
lookup[44].magicMultiplier = 1103974383956ull;
lookup[45].relevantBlockers = 22518341868716544ull;
lookup[45].magicMultiplier = 9008848824176900ull;
lookup[46].relevantBlockers = 9007336962655232ull;
lookup[46].magicMultiplier = 4504493182421008ull;
lookup[47].relevantBlockers = 18014673925310464ull;
lookup[47].magicMultiplier = 4526346848993408ull;
lookup[48].relevantBlockers = 2216338399232ull;
lookup[48].magicMultiplier = 588072256521371970ull;
lookup[49].relevantBlockers = 4432676798464ull;
lookup[49].magicMultiplier = 9010678181134336ull;
lookup[50].relevantBlockers = 11064376819712ull;
lookup[50].magicMultiplier = 2308094847768104960ull;
lookup[51].relevantBlockers = 22137335185408ull;
lookup[51].magicMultiplier = 9263904459322296480ull;
lookup[52].relevantBlockers = 44272556441600ull;
lookup[52].magicMultiplier = 2522157365330448384ull;
lookup[53].relevantBlockers = 87995357200384ull;
lookup[53].magicMultiplier = 175923141812352ull;
lookup[54].relevantBlockers = 35253226045952ull;
lookup[54].magicMultiplier = 14073920635289612ull;
lookup[55].relevantBlockers = 70506452091904ull;
lookup[55].magicMultiplier = 9376495530421518848ull;
lookup[56].relevantBlockers = 567382630219776ull;
lookup[56].magicMultiplier = 289358754259014153ull;
lookup[57].relevantBlockers = 1134765260406784ull;
lookup[57].magicMultiplier = 17859510308ull;
lookup[58].relevantBlockers = 2832480465846272ull;
lookup[58].magicMultiplier = 1163934286087717456ull;
lookup[59].relevantBlockers = 5667157807464448ull;
lookup[59].magicMultiplier = 9260526750960779520ull;
lookup[60].relevantBlockers = 11333774449049600ull;
lookup[60].magicMultiplier = 4632022723009414144ull;
lookup[61].relevantBlockers = 22526811443298304ull;
lookup[61].magicMultiplier = 10664523939491368968ull;
lookup[62].relevantBlockers = 9024825867763712ull;
lookup[62].magicMultiplier = 197914257334304ull;
lookup[63].relevantBlockers = 18049651735527936ull;
lookup[63].magicMultiplier = 1134696402530308ull;

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
            };
            auto attacks = evaluateDiagonalAttacks(blockers, i);
            auto key = blockers * lookup[i].magicMultiplier >> (64 - relevantBitsNum);
            lookup[i].attacks[key] = attacks;
        }
    }
}

RockMagicBitBoards::RockMagicBitBoards()
{lookup[0].relevantBlockers = 282578800148862ull;
    lookup[0].magicMultiplier = 9979976911964603521ull;
    lookup[1].relevantBlockers = 565157600297596ull;
    lookup[1].magicMultiplier = 6525718128876324996ull;
    lookup[2].relevantBlockers = 1130315200595066ull;
    lookup[2].magicMultiplier = 9808840134508939264ull;
    lookup[3].relevantBlockers = 2260630401190006ull;
    lookup[3].magicMultiplier = 297242113072989184ull;
    lookup[4].relevantBlockers = 4521260802379886ull;
    lookup[4].magicMultiplier = 9009441227735072ull;
    lookup[5].relevantBlockers = 9042521604759646ull;
    lookup[5].magicMultiplier = 7061732348479423488ull;
    lookup[6].relevantBlockers = 18085043209519166ull;
    lookup[6].magicMultiplier = 9531869710874841729ull;
    lookup[7].relevantBlockers = 36170086419038334ull;
    lookup[7].magicMultiplier = 144115329877443138ull;
    lookup[8].relevantBlockers = 282578800180736ull;
    lookup[8].magicMultiplier = 28148597451391488ull;
    lookup[9].relevantBlockers = 565157600328704ull;
    lookup[9].magicMultiplier = 9299935996747490656ull;
    lookup[10].relevantBlockers = 1130315200625152ull;
    lookup[10].magicMultiplier = 601437189048320ull;
    lookup[11].relevantBlockers = 2260630401218048ull;
    lookup[11].magicMultiplier = 986288249376768ull;
    lookup[12].relevantBlockers = 4521260802403840ull;
    lookup[12].magicMultiplier = 621575939218210817ull;
    lookup[13].relevantBlockers = 9042521604775424ull;
    lookup[13].magicMultiplier = 316663115285504ull;
    lookup[14].relevantBlockers = 18085043209518592ull;
    lookup[14].magicMultiplier = 318892933263872ull;
    lookup[15].relevantBlockers = 36170086419037696ull;
    lookup[15].magicMultiplier = 9404430817771937856ull;
    lookup[16].relevantBlockers = 282578808340736ull;
    lookup[16].magicMultiplier = 18144089358336ull;
    lookup[17].relevantBlockers = 565157608292864ull;
    lookup[17].magicMultiplier = 293860434841182272ull;
    lookup[18].relevantBlockers = 1130315208328192ull;
    lookup[18].magicMultiplier = 13619651741356288ull;
    lookup[19].relevantBlockers = 2260630408398848ull;
    lookup[19].magicMultiplier = 4901114862324680992ull;
    lookup[20].relevantBlockers = 4521260808540160ull;
    lookup[20].magicMultiplier = 1590620043476205572ull;
    lookup[21].relevantBlockers = 9042521608822784ull;
    lookup[21].magicMultiplier = 360294584444534820ull;
    lookup[22].relevantBlockers = 18085043209388032ull;
    lookup[22].magicMultiplier = 288235131587199488ull;
    lookup[23].relevantBlockers = 36170086418907136ull;
    lookup[23].magicMultiplier = 9511602550449672450ull;
    lookup[24].relevantBlockers = 282580897300736ull;
    lookup[24].magicMultiplier = 13840828296650063904ull;
    lookup[25].relevantBlockers = 565159647117824ull;
    lookup[25].magicMultiplier = 6917538970769833984ull;
    lookup[26].relevantBlockers = 1130317180306432ull;
    lookup[26].magicMultiplier = 9259964136022884356ull;
    lookup[27].relevantBlockers = 2260632246683648ull;
    lookup[27].magicMultiplier = 18019519186370624ull;
    lookup[28].relevantBlockers = 4521262379438080ull;
    lookup[28].magicMultiplier = 612500682415081472ull;
    lookup[29].relevantBlockers = 9042522644946944ull;
    lookup[29].magicMultiplier = 7323024600629249ull;
    lookup[30].relevantBlockers = 18085043175964672ull;
    lookup[30].magicMultiplier = 324541201394960896ull;
    lookup[31].relevantBlockers = 36170086385483776ull;
    lookup[31].magicMultiplier = 81346272564969570ull;
    lookup[32].relevantBlockers = 283115671060736ull;
    lookup[32].magicMultiplier = 9232380885378154576ull;
    lookup[33].relevantBlockers = 565681586307584ull;
    lookup[33].magicMultiplier = 290305732116613ull;
    lookup[34].relevantBlockers = 1130822006735872ull;
    lookup[34].magicMultiplier = 1154155294092693504ull;
    lookup[35].relevantBlockers = 2261102847592448ull;
    lookup[35].magicMultiplier = 2269392033808640ull;
    lookup[36].relevantBlockers = 4521664529305600ull;
    lookup[36].magicMultiplier = 433770539921703168ull;
    lookup[37].relevantBlockers = 9042787892731904ull;
    lookup[37].magicMultiplier = 2377905012103708944ull;
    lookup[38].relevantBlockers = 18085034619584512ull;
    lookup[38].magicMultiplier = 2323927799018032228ull;
    lookup[39].relevantBlockers = 36170077829103616ull;
    lookup[39].magicMultiplier = 8806968856612ull;
    lookup[40].relevantBlockers = 420017753620736ull;
    lookup[40].magicMultiplier = 580964914588484608ull;
    lookup[41].relevantBlockers = 699298018886144ull;
    lookup[41].magicMultiplier = 2306125661015769096ull;
    lookup[42].relevantBlockers = 1260057572672512ull;
    lookup[42].magicMultiplier = 77133292175900676ull;
    lookup[43].relevantBlockers = 2381576680245248ull;
    lookup[43].magicMultiplier = 1730529124042147840ull;
    lookup[44].relevantBlockers = 4624614895390720ull;
    lookup[44].magicMultiplier = 576495941071278084ull;
    lookup[45].relevantBlockers = 9110691325681664ull;
    lookup[45].magicMultiplier = 8804683530241ull;
    lookup[46].relevantBlockers = 18082844186263552ull;
    lookup[46].magicMultiplier = 5854716900319183105ull;
    lookup[47].relevantBlockers = 36167887395782656ull;
    lookup[47].magicMultiplier = 4900145367909173249ull;
    lookup[48].relevantBlockers = 35466950888980736ull;
    lookup[48].magicMultiplier = 11551768230796202624ull;
    lookup[49].relevantBlockers = 34905104758997504ull;
    lookup[49].magicMultiplier = 4901077590535607298ull;
    lookup[50].relevantBlockers = 34344362452452352ull;
    lookup[50].magicMultiplier = 35461934352449ull;
    lookup[51].relevantBlockers = 33222877839362048ull;
    lookup[51].magicMultiplier = 166637859573596224ull;
    lookup[52].relevantBlockers = 30979908613181440ull;
    lookup[52].magicMultiplier = 2332882218256433440ull;
    lookup[53].relevantBlockers = 26493970160820224ull;
    lookup[53].magicMultiplier = 288653688162746392ull;
    lookup[54].relevantBlockers = 17522093256097792ull;
    lookup[54].magicMultiplier = 4544284778987552ull;
    lookup[55].relevantBlockers = 35607136465616896ull;
    lookup[55].magicMultiplier = 9223653548372410624ull;
    lookup[56].relevantBlockers = 9079539427579068672ull;
    lookup[56].magicMultiplier = 2305914520428683394ull;
    lookup[57].relevantBlockers = 8935706818303361536ull;
    lookup[57].magicMultiplier = 288513501469761553ull;
    lookup[58].relevantBlockers = 8792156787827803136ull;
    lookup[58].magicMultiplier = 9223952725123826754ull;
    lookup[59].relevantBlockers = 8505056726876686336ull;
    lookup[59].magicMultiplier = 721704040118880546ull;
    lookup[60].relevantBlockers = 7930856604974452736ull;
    lookup[60].magicMultiplier = 35197558981766ull;
    lookup[61].relevantBlockers = 6782456361169985536ull;
    lookup[61].magicMultiplier = 2533446656197137ull;
    lookup[62].relevantBlockers = 4485655873561051136ull;
    lookup[62].magicMultiplier = 662042341528356866ull;
    lookup[63].relevantBlockers = 9115426935197958144ull;
    lookup[63].magicMultiplier = 2305845534671249794ull;


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
            };
            auto attacks = evaluateLineAttacks(blockers, i);
            auto key = blockers * lookup[i].magicMultiplier >> (64 - relevantBitsNum);
            lookup[i].attacks[key] = attacks;
        }
    }
}

uint64_t BishopMagicBitBoards::getAttacksFor(uint8_t fieldNum, uint64_t allpieces) const
{
    auto& l = lookup[fieldNum];
    unsigned key = ((l.relevantBlockers & allpieces) * l.magicMultiplier) >> (64u - relevantBitsNum);
    return l.attacks[key];
}

uint64_t RockMagicBitBoards::getAttacksFor(uint8_t fieldNum, uint64_t allpieces) const
{
    auto& l = lookup[fieldNum];
    unsigned key = ((l.relevantBlockers  & allpieces) * l.magicMultiplier) >> (64u - relevantBitsNum);
    return l.attacks[key];
}
