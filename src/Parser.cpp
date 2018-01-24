#include "Parser.hpp"
#include "EnglishConstants.hpp"

#include <iostream>

namespace StealthNLP {
    int parseSyllables(const std::string& word, std::vector<std::string>& syllables) noexcept {
        int syllableCount = 0;
        bool vowelFound = false, actingConsonantFound = false,
            prevVowelFound = false, prevActingConsonantFound = false;
        // Maintain endpoints for syllables
        std::string::const_iterator syllableBegin = word.begin();
        // GO!
        for (std::string::const_iterator letter = word.begin(); letter != word.end(); ++letter) {
            // Remember whether the previous letter was a vowel and check if this letter is a vowel.
            prevVowelFound = vowelFound;
            prevActingConsonantFound = actingConsonantFound;
            // Check if this vowel is going to act like a consonant.
            actingConsonantFound = VOWEL_PAIRS.count(*letter) && VOWEL_PAIRS.at(*letter).count(*(letter - 1));
            vowelFound = VOWELS.count(*letter) && !actingConsonantFound;

            // DEBUG
            // if (actingConsonantFound) std::cout << "Acting consonant found at: " << *letter << '\n';
            // else if (vowelFound) std::cout << "Vowel found at: " << *letter << '\n';
            // else std::cout << "Consonant found at: " << *letter << '\n';
            // END DEBUG

            // If this letter is a consonant (or acting consonant) but the last letter was a vowel, end the syllable.
            if (actingConsonantFound) {
                syllables.emplace_back(syllableBegin, letter);

                // DEBUG
                // std::cout << "Added syllable (due to acting consonant): " << syllables.back() << '\n';
                // END DEBUG
                
                ++syllableCount;
                syllableBegin = letter;
                // Scan until the next vowel
                prevVowelFound = true;
                while (!vowelFound) {
                    vowelFound = VOWELS.count(*++letter);
                }
                syllables.emplace_back(syllableBegin, letter);

                // DEBUG
                // std::cout << "Added syllable (after acting consonant): " << syllables.back() << '\n';
                // END DEBUG
                
                ++syllableCount;
                syllableBegin = letter;
            } else if (prevVowelFound && !vowelFound ) {
                // Make sure double consonant is NOT due to an acting consonant
                if (!VOWELS.count(*syllableBegin) && !VOWELS.count(*(syllableBegin + 1)) && syllableCount != 0) {

                    // DEBUG
                    // std::cout << "Found double consonant: " << *syllableBegin << *(syllableBegin + 1) << '\n';
                    // END DEBUG
                    
                    // In case of a double consonant, append the first one to the previous syllable
                    syllables.back() += *syllableBegin++;
                }
                syllables.emplace_back(syllableBegin, letter);

                // DEBUG
                // std::cout << "Added syllable: " << syllables.back() << '\n';
                // END DEBUG
                
                ++syllableCount;
                // Start a new syllable
                syllableBegin = letter;
            }
        }
        // Handle last letter
        if (!VOWELS.count(word.back()) || (VOWELS.count(word[word.size() - 3]) && word.back() == 'e')) {
            // If we end on a consonant (or 'e' in special cases), add it to the previous syllable.
            syllables.back() += std::string(syllableBegin, word.end());
        } else {
            // Otherwise add the syllable remaining in the buffer
            syllables.emplace_back(syllableBegin, word.end());
        }
        return syllableCount;
    }
} /* StealthNLP */
