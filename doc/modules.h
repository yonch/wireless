/**
 * @anchor modules_anchor
 *
 * @defgroup packetgen Packet Generators
 * @brief Generate packets for tests
 *
 * @defgroup protocols Link Layer Protocols
 * @brief Specify how many symbols to transmit before ACK/NAK
 *
 * @defgroup codes Error Correcting Codes
 * @brief Encoders and Decoders
 * @{
 *     @defgroup fountain Fountain Codes
 *     @brief LT and Raptor codes
 *
 *     @defgroup ldpc LDPC Codes
 *     @brief Low Density Parity Check codes
 *
 *     @defgroup puncturing Puncturing
 *     @brief Methods of choose a subset of symbols to transmit, to get higher coding rate
 *
 *     @defgroup spinal Spinal Codes
 *     @brief Spinal codes
 *
 *     @defgroup strider Strider
 *     @brief Implementation of layered approach to coding
 *
 *     @defgroup turbo Turbo Code
 *     @brief Turbo code
 *
 *     @defgroup inference Inference library
 *     @brief Functions for making inference on graphs and hidden markov models
 *     @{
 *         @defgroup bp Belief Propagation
 *         @brief Data-structures and algorithms for efficient belief propagation
 *
 *         @defgroup hmm Hidden Markov Models
 *         @brief Inference on HMMs (exploring the state tree, pruning, backtracking)
 *     @}
 *
 * @}
 *
 * @defgroup mappers Mappers
 * @brief Translate codeword bits to symbols
 *
 * @defgroup channels Channel simulators
 * @brief Simulators for AWGN, BSC, etc. add noise to transmitted symbols
 *
 * @defgroup demappers Demappers
 * @brief Process received symbols, producing likelihoods
 *
 * @defgroup detectors Detectors of successful decoding
 * @brief Detect whether packet decode was successful, or more symbols are needed
 *
 * @defgroup util Utilities
 * @brief Support algorithms
 * @{
 *     @defgroup hashes Hashes
 *     @brief Hash function implementations
 * @}
 */
