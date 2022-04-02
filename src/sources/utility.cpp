#define maxAttributes 10
#include <bits/stdc++.h>
#include <algorithm>
/**
 * Feed it a list of positions and bools, it will return true if all are true,
 * else returns false;
 * @author Francisco Romero
 * @param toCheck the string to check
 * @param values first value is position, second value is true or false
 * @return
 */
int checkAnd(std::bitset<maxAttributes> toCheck, std::vector<std::pair<int, int>> values) {
    for (int i = 0; i < values.size(); i++){
        if (toCheck.test(values[i].first) != values[i].second) {
            //printf("rejecting and: %s = %d at %d %d\n", toCheck.to_string().c_str(), toCheck.test(i), values[i].first, values[i].second);
            return 0;
        }
        //printf("continuing an: %s = %d at %d %d\n", toCheck.to_string().c_str(), toCheck.test(i), values[i].first, values[i].second);
    }
    //std::cout << "Made it throug and\n" << std::endl;
    return 1;
}
/**
 * Runs through the ands, if any of them are true returns true
 * @param toCheck the binary value containing the state to check against
 * @param values the values to try to match
 * @return true if matches atleast one of the OR's, false if no
 */
int checkOr(std::bitset<maxAttributes> toCheck, std::vector<std::vector<std::pair<int, int>>> values) {
    //std::cout << "Values: " << values.size() << std::endl;
    for (int i = 0; i < values.size(); i++) {
        if (checkAnd(toCheck, values[i])) {
            //std::cout << "Fuck Reject\n";
            return 1;
        }
    }
    //std::cout << "Fuck accept\n";
    return 0;
}

void testing() {

    //Construct texting binary numbers;
    std::vector<std::pair<int, int>> test1;
    std::vector<std::pair<int, int>> test2;
    std::bitset<maxAttributes> bTest("1001");

    /** test1 test data: "1111" */
    test1.push_back(std::make_pair(0, 1));
    test1.push_back(std::make_pair(1, 1));
    test1.push_back(std::make_pair(2, 1));
    test1.push_back(std::make_pair(3, 1));

    /** test2 test data: "1001" */
    test2.push_back(std::make_pair(0, 1));
    test2.push_back(std::make_pair(1, 0));
    test2.push_back(std::make_pair(2, 0));
    test2.push_back(std::make_pair(3, 1));

}
void testing2(std::vector<std::vector<std::pair<int,int>>> hardConstraints) {

    //Construct texting binary numbers;
    std::vector<std::pair<int, int>> test1;
    std::vector<std::pair<int, int>> test2;
    //std::bitset<maxAttributes> bTest("1001");

    for (int i = 0; i < hardConstraints.size(); i++){
        for (int j = 0; j < hardConstraints[i].size(); j++){
            std::cout << hardConstraints[i][j].first << ' ' << hardConstraints[i][j].second << std::endl;
        }
        std::cout << std::endl;
    }
    std::vector<int> toTest;
    for(int i = 0; i < 5; i++)
        toTest.emplace_back(i);

    for(int i = 0; i < toTest.size();i++){
        std::bitset<maxAttributes> bTest(toTest[i]);
        std::string s = bTest.to_string();
        //std::reverse(s.begin(),s.end());
        //std::cout << "Test S: " << s << std::endl;
        int nolan = checkOr(bTest, hardConstraints);
        if(nolan){
            std::cout << "Reject: " << bTest.to_string() << std::endl;
        }
        else{
            std::cout << "Accept: " << bTest.to_string() << std::endl;
        }
        bTest = 0;
        s = "";
    }

}

/**
 * finds all states which violate the hard constraints
 * @param attributeCount how many attributes in the state
 * @param hardConstraints the constraints themselves
 * @return a list of all states violating the constraints, in integer form.
 */
std::vector<int> blacklistFunction(int attributeCount, std::vector<std::vector<std::pair<int,int>>> hardConstraints) {
    /*
    for (int i = 0; i < hardConstraints.size(); i++){
        for (int j = 0; j < hardConstraints[i].size(); j++){
            std::cout << hardConstraints[i][j].first << ' ' << hardConstraints[i][j].second << std::endl;
        }
        std::cout << std::endl;
    }
    */
    std::vector<int> blacklist;
    for(int i = 0; i < attributeCount*attributeCount;i++){
        std::bitset<maxAttributes> bTest(i);
        std::string s = bTest.to_string();
        //std::reverse(s.begin(),s.end());
        //std::cout << "Test S: " << s << std::endl;
        if(checkOr(bTest, hardConstraints)){
            //std::cout << "Blacklist: " << i << ' ' << bTest.to_string() << std::endl;
            blacklist.emplace_back(i);
        }
        else{
            //std::cout << "Allow: " << i << ' ' << bTest.to_string() << std::endl;
        }
    }
    return blacklist;
}

 /**
  * Calculates Penalty Logic
  * @param attributeNames names of attributes, for later displaying in a pretty format
  * @param penaltyStrings propositional function, to display later
  * @param blacklist states to skip over
  * @param penalties penalty conditions to check for
  * @param penaltyStack which term belongs to which penalty
  * @param penaltyCosts costs of penaltiesAndProbabilities
  * @return returns the matrix, to be formatted by william daniel hiromoto for display
  */
std::vector<std::vector<std::string>> penaltiesFunction(std::unordered_map<std::string, std::pair<std::string,std::string>> attributeNames, std::vector<std::string> penaltyStrings, std::vector<int> blacklist, std::vector<std::vector<std::pair<int,int>>> penalties, std::vector<int> penaltyStack, std::vector<int> penaltyCosts) {
    std::vector<std::vector<std::string>> matrix(attributeNames.size()*attributeNames.size()-blacklist.size()+1, std::vector<std::string>(penaltyStack.size()+1));
    std::string toInsert;
    //std::cout << "checking size: " << penaltyStack.size();
    matrix[0][0] = "state";
    for(int i = 0; i < penaltyStrings.size();i++){
        matrix[0][i+1] = penaltyStrings[i];
        matrix[0][i+2] = "total penalty";;
    }
    int iteration = 0; // for first column to deal with blacklisting
    std::cout << std::endl;
    for(int i = 0; i < attributeNames.size()*attributeNames.size()-blacklist.size(); i++){

        for(int j = 0; j < penaltyStack.size(); j++){
            //std::cout << "Science: " << penaltyCosts[j] << std::endl;
            matrix[i + 1][j + 1] = "0";
        }
        //std::cout << std::endl;
    }
    for(int i = 0; i < attributeNames.size()*attributeNames.size();i++){
        std::vector<int> detected(penaltyCosts.size(), 0);
        std::bitset<maxAttributes> state(i);
        std::string s = state.to_string();
        int total = 0;
        // should have made a set to check if xyz is blacklisted but have this spaghetti insteal
        for(int j : blacklist)
            if(i == j)
                goto skip;
        toInsert = std::bitset<maxAttributes>(i).to_string().substr(maxAttributes-attributeNames.size(), maxAttributes);
        iteration++;
        matrix[iteration][0] = toInsert;

        for(int j = 0; j < penalties.size(); j++){
            if(checkAnd(state, penalties[j])){
                detected[penaltyStack[j]] = 1;
                matrix[iteration][penaltyStack[j]+1] = std::to_string(penaltyCosts[penaltyStack[j]]);

                //std::cout << "Detected Penalty for: "
            }
        }
        for(int j = 0; j < detected.size(); j++){
            //std::cout << detected[j] << ' ';
            if(detected[j]){
                total += penaltyCosts[j];
            }
        }
        matrix[iteration][detected.size()+1] = std::to_string(total);
        std::cout << std::endl;
        //matrix[i][penaltiesAndProbabilities.size()-1] = std::to_string(total);
        //std::cout << "Detect Test ";
        for(int j = 0; j < penaltyStack.size(); j++){
            //int ree = std:stoi
        }
        skip:;
    }
    for(int i = 0; i < attributeNames.size()*attributeNames.size()-blacklist.size()+1; i++){
        for(int j = 0; j < penaltyStack.size(); j++){
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    return matrix;
}

/**
 * Calculates possibilistic Logic
 * @param attributeNames names of attributes, for later displaying in a pretty format
 * @param penaltyStrings propositional function, to display later
 * @param blacklist states to skip over
 * @param penalties penalty conditions to check for
 * @param penaltyStack which term belongs to which penalty
 * @param penaltyCosts cost for ith term
 * @return returns the matrix, to be formatted by william daniel hiromoto for display
 */
std::vector<std::vector<std::string>> possibilisticFunction(std::unordered_map<std::string, std::pair<std::string,std::string>> attributeNames, std::vector<std::string> penaltyStrings, std::vector<int> blacklist, std::vector<std::vector<std::pair<int,int>>> penalties, std::vector<int> penaltyStack, std::vector<int> penaltyCosts) {
    std::vector<std::vector<std::string>> matrix(attributeNames.size()*attributeNames.size()-blacklist.size()+1, std::vector<std::string>(penaltyStack.size()+1));
    std::string toInsert;
    //std::cout << "checking size: " << penaltyStack.size();
    matrix[0][0] = "state";
    for(int i = 0; i < penaltyStrings.size();i++){
        matrix[0][i+1] = penaltyStrings[i];
        matrix[0][i+2] = "total desirabily";
    }
    int iteration = 0; // for first column to deal with blacklisting
    std::cout << std::endl;
    for(int i = 0; i < attributeNames.size()*attributeNames.size()-blacklist.size(); i++){

        for(int j = 0; j < penaltyStack.size(); j++){
            //std::cout << "Science: " << penaltyCosts[j] << std::endl;
            matrix[i + 1][j + 1] = "1";
        }
        //std::cout << std::endl;
    }
    for(int i = 0; i < attributeNames.size()*attributeNames.size();i++){
        std::vector<int> detected(penaltyCosts.size(), 0);
        std::bitset<maxAttributes> state(i);
        std::string s = state.to_string();
        double total = 1;
        // should have made a set to check if xyz is blacklisted but have this spaghetti insteal
        for(int j : blacklist)
            if(i == j)
                goto skip;
        toInsert = std::bitset<maxAttributes>(i).to_string().substr(maxAttributes-attributeNames.size(), maxAttributes);
        iteration++;
        matrix[iteration][0] = toInsert;

        for(int j = 0; j < penalties.size(); j++){
            if(checkAnd(state, penalties[j])){
                detected[penaltyStack[j]] = 1;
                matrix[iteration][penaltyStack[j]+1] = std::to_string(1-(double)penaltyCosts[penaltyStack[j]]/10.0);

                //std::cout << "Detected Penalty for: "
            }
        }
        for(int j = 0; j < detected.size(); j++){
            //std::cout << detected[j] << ' ';

            if(detected[j]){
                total = fmin(total, 1-(double)penaltyCosts[j]/10.0);
                //cout << "science: total"
                //total = penaltyCosts[j];
            }
            std::cout << total << ' ';
        }
        matrix[iteration][detected.size()+1] = std::to_string(total);
        std::cout << std::endl;
        //matrix[i][penaltiesAndProbabilities.size()-1] = std::to_string(total);
        //std::cout << "Detect Test ";
        for(int j = 0; j < penaltyStack.size(); j++){
            //int ree = std:stoi
        }
        skip:;
    }
    for(int i = 0; i < attributeNames.size()*attributeNames.size()-blacklist.size()+1; i++){
        for(int j = 0; j < penaltyStack.size(); j++){
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    return matrix;
}

/**
 * Calculates qualitative Logic
 * @param attributeNames names of attributes, for later displaying in a pretty format
 * @param penaltyStrings propositional function, to display later
 * @param blacklist states to skip over
 * @param penalties penalty conditions to check for
 * @param penaltyStack which term belongs to which penalty
 * @param penaltyCosts costs of penaltiesAndProbabilities
 * @param qualitativeColumn which line goes to which column
 * @param qualitativeCost cost of i line
 * @return returns the matrix, to be formatted by william daniel hiromoto for display
 */
std::vector<std::vector<std::string>> qualitativeFunction(std::unordered_map<std::string, std::pair<std::string,std::string>> attributeNames, std::vector<std::string> penaltyStrings, std::vector<int> blacklist, std::vector<std::vector<std::pair<int,int>>> penalties, std::vector<int> penaltyStack, std::vector<int> penaltyCosts, std::vector<int> qualitativeColumn, std::vector<int> qualitativeCost) {
    std::unordered_set<int> distinct;
    for(int i = 0; i < qualitativeCost.size(); i++)
        distinct.insert(qualitativeColumn[i]);
    int distinctCols = distinct.size();
    std::vector<std::string> columnHeaders(distinctCols, "");
    for(int i = 0; i < qualitativeColumn.size(); i++){
        columnHeaders[qualitativeColumn[i]-1] += penaltyStrings[i].substr(0,penaltyStrings[i].size()-3) + '>';
    }
    std::cout << "science distinct size: " << distinctCols << std::endl;

    std::vector<std::vector<std::string>> matrix(attributeNames.size()*attributeNames.size()-blacklist.size()+1, std::vector<std::string>(distinctCols+1));

    std::string toInsert;
    //std::cout << "checking size: " << penaltyStack.size();
    matrix[0][0] = "state";
    for(int i = 0; i < distinctCols;i++){
        matrix[0][i+1] = columnHeaders[i].substr(0,columnHeaders[i].size()-1);
    }
    std::cout << std::endl;
    std::cout << "Header test: " << distinctCols << std::endl;
    for(int i = 0; i <= distinctCols;i++){
        std::cout << matrix[0][i] << ' ';
    }
    int iteration = 0; // for first column to deal with blacklisting
    std::cout << std::endl;

    for(int i = 0; i < attributeNames.size()*attributeNames.size()-blacklist.size(); i++){
        matrix[i+1][0] = "binary";
        for(int j = 0; j < distinctCols; j++){
            //std::cout << "Science: " << penaltyCosts[j] << std::endl;
            matrix[i + 1][j + 1] = "inf";
        }
        //std::cout << std::endl;
    }
    for(int i = 0; i < attributeNames.size()*attributeNames.size();i++){
        std::bitset<maxAttributes> state(i);
        int total = 0;
        // should have made a set to check if xyz is blacklisted but have this spaghetti insteal
        for(int j : blacklist)
            if(i == j)
                goto skip;
        toInsert = std::bitset<maxAttributes>(i).to_string().substr(maxAttributes-attributeNames.size(), maxAttributes);
        iteration++;
        matrix[iteration][0] = toInsert;

        for(int j = 0; j < penalties.size(); j++){
            if(checkAnd(state, penalties[j])){
                matrix[iteration][qualitativeColumn[j]] = std::to_string(qualitativeCost[j]);
            }
        }
        skip:;
    }
    std::cout << std::endl;
    for(int i = 0; i < attributeNames.size()*attributeNames.size()-blacklist.size()+1; i++){
        for(int j = 0; j <= distinctCols; j++){
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    return matrix;
}
/**
 * 0 for penalty
 * 1 for possibilistic
 * 2 for qualitative
 */
std::vector<std::vector<std::string>> feasbility(std::vector<std::vector<std::string>> matrix, int which){
    std::vector<std::vector<std::string>> toSend;
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[i].size(); j++){
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "Fuck Fuck " << std::endl << std::endl;
    toSend.emplace_back(matrix[0]);
    if(which == 0){
        for(int i = 1; i < matrix.size(); i++)
            if(matrix[i][matrix[i].size()-2] != "0"){
                std::vector<std::string> fuck = {matrix[i][0]};
                for(int j = 0; j < toSend[i].size(); j++){
                    //fuck.emplace_back()

                }
            }
            //toSend.emplace_back(matrix[i]);
        }
    for(int i = 0; i < toSend.size(); i++){
        for(int j = 0; j < toSend[i].size(); j++){
            std::cout << toSend[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    return toSend;
}