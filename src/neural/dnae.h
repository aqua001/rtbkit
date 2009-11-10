/* dnae.h                                                          -*- C++ -*-
   Jeremy Barnes, 4 November 2009
   Copyright (c) 2009 Jeremy Barnes.  All rights reserved.

   Denoising autoencoder training code.
*/

#ifndef __jml__neural__dnae_h__
#define __jml__neural__dnae_h__


#include "twoway_layer.h"
#include "layer_stack.h"
#include "stats/distribution.h"
#include <vector>


namespace ML {


/*****************************************************************************/
/* DNAE_TRAINER                                                              */
/*****************************************************************************/

struct DNAE_Trainer {

    void
    train_dnae(Layer_Stack<Twoway_Layer> & stack,
               const std::vector<distribution<float> > & training_data,
               const std::vector<distribution<float> > & testing_data,
               const Configuration & config,
               Thread_Context & thread_context);
    
    
    std::pair<double, double>
    test_dnae(Layer_Stack<Twoway_Layer> & stack,
              const std::vector<distribution<float> > & data,
              float prob_cleared,
              Thread_Context & thread_context,
              int verbosity);

    
    /** Backpropagate the given example.  The gradient will be acculmulated in
        the output.  Fills in the errors for the next stage at input_errors. */
    void backprop_example(const distribution<double> & outputs,
                          const distribution<double> & output_deltas,
                          const distribution<double> & inputs,
                          distribution<double> & input_deltas,
                          Parameters & updates) const;
    
    /** Inverse direction backpropagation of the given example.  Again, the
        gradient will be acculmulated in the output.  Fills in the errors for
        the next stage at input_errors. */
    void ibackprop_example(const distribution<double> & outputs,
                           const distribution<double> & output_deltas,
                           const distribution<double> & inputs,
                           distribution<double> & input_deltas,
                           Parameters & updates) const;

    /** Trains a single iteration on the given data with the selected
        parameters.  Returns a moving estimate of the RMSE on the
        training set. */
    std::pair<double, double>
    train_iter(const std::vector<distribution<float> > & data,
               float prob_cleared,
               Thread_Context & thread_context,
               int minibatch_size, float learning_rate,
               int verbosity,
               float sample_proportion,
               bool randomize_order);

    /** Tests on the given dataset, returning the exact and noisy RMSE.  If
        data_out is non-empty, then it will also fill it in with the
        hidden representations for each of the inputs (with no noise added).
        This information can be used to train the next layer. */
    std::pair<double, double>
    test_and_update(const std::vector<distribution<float> > & data_in,
                    std::vector<distribution<float> > & data_out,
                    float prob_cleared,
                    Thread_Context & thread_context,
                    int verbosity) const;

    /** Tests on the given dataset, returning the exact and noisy RMSE. */
    std::pair<double, double>
    test(const std::vector<distribution<float> > & data,
         float prob_cleared,
         Thread_Context & thread_context,
         int verbosity) const
    {
        std::vector<distribution<float> > dummy;
        return test_and_update(data, dummy, prob_cleared, thread_context,
                               verbosity);
    }
};

    
} // namespace ML

#endif /* __jml__neural__dnae_h__ */
