//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"

uint32_t numLabels;
uint32_t numVertices;
cardStat* labelData;
uint32_t** inPaths;
uint32_t** outPaths;
std::vector<std::pair<uint32_t, char>> queryVector;


SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;


}

void SimpleEstimator::prepare() {
    numLabels = graph.get()->getNoLabels();
    numVertices = graph.get()->getNoVertices();

    inPaths = new uint32_t*[numLabels];
    outPaths = new uint32_t*[numLabels];
    labelData = new cardStat[numLabels];

    for(auto i = 0; i < numLabels; i ++) {
        inPaths[i] = new uint32_t[numVertices];
        outPaths[i] = new uint32_t[numVertices];
        for(auto j = 0; j < numVertices; j ++) {
            inPaths[i][j] = 0;
            outPaths[i][j] = 0;
        }
    }

    for(auto i = 0; i < numLabels; i ++) {
        for(auto p: graph.get()->adj[i]) {
            inPaths[i][p.second] ++;
            outPaths[i][p.first] ++;
        }
    }

    uint32_t numPaths = 0;
    uint32_t numIn = 0;
    uint32_t numOut = 0;

    for(auto i = 0; i < numLabels; i ++) {
        for(auto j = 0; j < numVertices; j ++) {
            numPaths += inPaths[i][j];
            if(inPaths[i][j] > 0)
                numIn ++;
            if(outPaths[i][j] > 0)
                numOut ++;

        }
        labelData[i] = {numOut, numPaths, numIn};
        numPaths = 0;
        numIn = 0;
        numOut = 0;
    }
}

void treeToList(RPQTree* q) {
    if(q->isConcat()) {
        treeToList(q->left);
        treeToList(q->right);
    }
    else if(q->isLeaf()) {
        auto label = q->data.substr(0, q->data.size() - 1);
        std::stringstream geek(label);
        uint32_t labelInt;
        geek >> labelInt;
        queryVector.push_back(std::make_pair(labelInt, q->data.at(q->data.size() - 1)));
    }
}

cardStat reverse(cardStat c) {
    return {c.noIn, c.noPaths, c.noOut};
}

cardStat SimpleEstimator::estimate(RPQTree *q) {
       treeToList(q);

    if(queryVector.size()==0)
    {
        queryVector.clear();
        return cardStat{0,0,0};
    }
    else if(queryVector.size()==1)
    {
        cardStat onlyOne=labelData[queryVector[0].first];
        queryVector.clear();
        if(queryVector[0].second == '+')
            return onlyOne;
        else return reverse(onlyOne);
    }
    else
    {
        cardStat left;
        if(queryVector[0].second == '+')
            left = labelData[queryVector[0].first];
        else left = reverse(labelData[queryVector[0].first]);

        uint32_t total = (left.noIn + left.noOut) / 2;
        for(int i=1; i<queryVector.size();i++)
        {
            cardStat right;
            if(queryVector[i].second == '+')
                right = labelData[queryVector[i].first];
            else right = reverse(labelData[queryVector[i].first]);

            uint32_t in = (left.noIn)/ 4;
            uint32_t out = (right.noOut) / 4;
            total /= 4;
            auto paths = std::min(2 * left.noPaths * right.noPaths / (right.noIn + right.noOut + total),
                                  2 * left.noPaths * right.noPaths / (left.noOut + left.noIn + total));
            cardStat processed = cardStat{std::min(out, paths), paths, std::min(in, paths)};
            left = processed;
        }
        queryVector.clear();
        return left;
    }
}
