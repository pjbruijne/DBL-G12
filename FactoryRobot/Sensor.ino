/**
 * Distance calculated using the p-norm. Used by our function below to calculate the distance between the raw data and the preset colors.
 */
float pNormDistance(uint16_t vector1[], uint16_t vector2[], int size,int p) {
  float x = 0;
  if (p!=0) {
    for (int i = 0; i < size; i++) {
      float a = (float) vector1[i];
      float b = (float) vector2[i];
      x+= pow(abs(b-a),p);
    }
    return pow(x,1.0f/p);
  }
  else {
    float a = (float) vector1[0];
    float b = (float) vector2[0];
    x = abs(b-a);
    for (int i = 1; i < size; i++) {
      a = (float) vector1[i];
      b = (float) vector2[i];
      float y = abs(b-a);
      x = max(x,y);
    }
    return x;
  }
  
}

/**
 * Our method for getting the disk color based on the rgb values provided by the sensor.
 * 
 * @param r The value of the color red
 * @param g The value of the color green
 * @param b The value of the color blue
 * @param c The value of clear
 * @param lux The light level
 * @return The color seen by the sensor index with our own sequence
 */
int getColor(uint16_t r, uint16_t g, uint16_t b) {
  int p = 2;  // we use euclidean distance, but we could easily switch to manhattan or other preset distance types
  uint16_t rawData[] = {r,g,b}; // the rgb of input in an array
  uint16_t black[] = {85,85,70};  // the values of black 2
  uint16_t green[] = {230,370,215};  // the values of green 3
  uint16_t white[] = {740,900,850};  // the values of white 1
  uint16_t belt[] = {60,60,45}; // the values of the belt 0
  float blackDist = pNormDistance(rawData,black,3,p); // the euclidean distance between the sensor rgb and black
  float greenDist = pNormDistance(rawData,green,3,p); // the euclidean distance between the sensor rgb and green
  float whiteDist = pNormDistance(rawData,white,3,p); // the euclidean distance between the sensor rgb and white
  float beltDist = pNormDistance(rawData,belt,3,p);  // the euclidean distance between the sensor rgb and belt
  float minDist = min(min(beltDist,blackDist),min(greenDist,whiteDist));
  boolean isWhite = true;
  for (int i = 0; i < 3; i++) {
    if (rawData[i] < white[i]) {
      isWhite = false;
      break;
    }
  }
  if (isWhite) {
    return 1;
  }
  else if (minDist < acceptance) { // if there is a distance smaller than the acceptance variable
    if (whiteDist == minDist) {  // if white has smallest distance to raw data
      return 1; // disk is white
    }
    else if (blackDist == minDist) {  // if black has smallest distance to raw data 
      return 2; // disk is black
    }
    else if (greenDist == minDist) {  // if green has smallest distance to raw data
      return 3; // disk is green
    }
    else return 0;  // else belt has smallest distance to raw data by process of elimination, no disk present
  }
  else return -1; // if smallest distance is not smaller than acceptance, return -1 meaning something wrong is on the belt
}
