#include "Gesture_predictor.hpp"
#include "Constants.hpp"
#include <cstdio>

namespace {
// State for the averaging algorithm we're using.
float prediction_history[kGestureCount][kPredictionHistoryLength] = {};
int prediction_history_index = 0;
int prediction_suppression_count = 0;
}  // namespace

// Return the result of the last prediction
// 0: wing("W"), 1: ring("O"), 2: slope("angle"), 3: unknown
int PredictGesture(float* output) {
  // Record the latest predictions in our rolling history buffer.
  for (int i = 0; i < kGestureCount; ++i) {
    prediction_history[i][prediction_history_index] = output[i];
  }
  // Figure out which slot to put the next predictions into.
  ++prediction_history_index;
  if (prediction_history_index >= kPredictionHistoryLength) {
    prediction_history_index = 0;
  }
  // Average the last n predictions for each gesture, and find which has the
  // highest score.
  int max_predict_index = -1;
  float max_predict_score = 0.0f;
  for (int i = 0; i < kGestureCount; i++) {
    float prediction_sum = 0.0f;
    for (int j = 0; j < kPredictionHistoryLength; ++j) {
      prediction_sum += prediction_history[i][j];
    }
    const float prediction_average = prediction_sum / kPredictionHistoryLength;
    if ((max_predict_index == -1) || (prediction_average > max_predict_score)) {
      max_predict_index = i;
      max_predict_score = prediction_average;
    }
  }
  // If there's been a recent prediction, don't trigger a new one too soon.
  if (prediction_suppression_count > 0) {
    --prediction_suppression_count;
  }
  // If we're predicting no gesture, or the average score is too low, or there's
  // been a gesture recognised too recently, return no gesture.
  if ((max_predict_index == kNoGesture) ||
      (max_predict_score < kDetectionThreshold) ||
      (prediction_suppression_count > 0)) {
    return kNoGesture;
  } else {
    // Reset the suppression counter so we don't come up with another prediction
    // too soon.
    prediction_suppression_count = kPredictionSuppressionDuration;
    return max_predict_index;
  }
}