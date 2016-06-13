#include <iostream>
#include <memory>
#include "conf.hh"
#include "io.hh"
#include "HashGram.hh"


std::vector<std::pair<std::shared_ptr<HashGram_t<int> >, float> >
component_models_and_weights(const std::string inputlist) {
        std::vector<std::pair<std::shared_ptr<HashGram_t<int> >, float> > mwpairs;
        Vocabulary all_vocabs;

        std::vector<std::string> split_strings1;
        str::split(&inputlist, ";", false, &split_strings1);
        for (auto model_weight: split_strings1) {
                std::vector<std::string> split_strings2;
                //std::cout << model_weight << std::endl;
                str::split(&model_weight, ",", false, &split_strings2);
                const float weight = std::stof(split_strings2[1]);
                const std::string model_name = split_strings2[0];
                std::cout << "Model " << model_name
                          << " " << weight << std::endl;
                std::shared_ptr<HashGram_t<int> > cur_ngram(new HashGram_t<int>);
                all_vocabs.copy_vocab_to(*cur_ngram);
                cur_ngram->set_oov("<UNK>");
                io::Stream in(model_name, "r");
                cur_ngram->read(in.file, 0);
                cur_ngram->copy_vocab_to(all_vocabs);

                auto mod_w_pair = std::make_pair(std::move(cur_ngram), weight);
                mwpairs.push_back(std::move(mod_w_pair));

        }

        // Make sure that all vocabs are equal (The first ones may
        // miss words present only at later ones)
        for (auto mwpair: mwpairs) {
                all_vocabs.copy_vocab_to(*(mwpair.first));
        }

        return mwpairs;
}

int main (int argc, char *argv[]) {
        // Assumes that the input is a backoff model! FIXME: Add assert
        conf::Config config;
        config("Usage: simpleinterpolate2arpa \"lm1_in.arpa,weight1;lm2_in.arpa,weight2\" out_interpolate.arpa\nCreate an arpa LM that approximately contains the linear interpolation of the input ngrams (exact solution cannot be reduced to arpa format).\n\nCaveats: \n\t* Only tests for this code are what is in unit tests (further testing needed, use at your own risk).\n\t* No computational optimizations applied (FIXME)\n\t* currently only supports interpolation between two models (FIXME)\n");
        config.parse(argc, argv,2);

        auto models_and_weigths = component_models_and_weights(config.arguments.at(0));

        // FIXME: Only interpolates the two first models
        models_and_weigths[0].first->fake_interpolate(
                *(models_and_weigths[1].first.get()), models_and_weigths[0].second);

        io::Stream out(config.arguments.at(1), "w");
        models_and_weigths[0].first->write(out.file);
}