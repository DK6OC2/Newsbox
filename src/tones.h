// sound routines

#define BUZZER_PIN 27 // D27 - GPIO27
#define BUZZER_PWN_CHANNEL 0

struct ToneQueue
{                        // queue for tones to play
  double freq;           // pitch in Hertz, 0 for off
  unsigned int duration; // tone duration in milliseconds
};
const int maxTones = 50; // maximum count of tones to play
ToneQueue tones[maxTones];
int toneCount = 0;
unsigned long last_tone_start = 0;
bool tone_initialized = false;

// play tones to play
void play_tones()
{
  if (last_tone_start == 0 && toneCount == 0)
    return;              // nothing to do
  if (!tone_initialized) // LEDC not yet prepared
  {
    ledcSetup(BUZZER_PWN_CHANNEL, 1000, 8);        // PWM channel 0, freq 1000 Hz, 8 bit resolution
    ledcAttachPin(BUZZER_PIN, BUZZER_PWN_CHANNEL); // activate PWM
    tone_initialized = true;
  }
  unsigned long currentMillis = millis();
  if (last_tone_start > 0) // if a tone is already playing
  {
    if (currentMillis - last_tone_start >= tones[0].duration) // if duration is over
    {
      last_tone_start += tones[0].duration;
      ledcWriteTone(BUZZER_PWN_CHANNEL, 0); // switch off the playing tone
      for (int i = 1; i < toneCount; i++)   // move all tones forward by one
        tones[i - 1] = tones[i];
      tones[toneCount].freq = 0; // overwrite values of the last (slipped) entry
      tones[toneCount].duration = 0;
      toneCount--;
      if (toneCount == 0) // if nothing is left
      {
        ledcWriteTone(BUZZER_PWN_CHANNEL, 0); // switch off playing tone
        last_tone_start = 0;
      }
    }
    else // tone still continues
      return;
  }
  if (toneCount > 0) // if buffered tones are still available
  {
    #if MUTE_BUZZER == true
      ledcWriteTone(BUZZER_PWN_CHANNEL, 0);
    #else
      ledcWriteTone(BUZZER_PWN_CHANNEL, tones[0].freq);
    #endif
    // Serial.print ("Play tone: ");  Serial.println (tones[0].freq);
    if (last_tone_start == 0)
      last_tone_start = currentMillis;
  }
}

// place a tone in the queue
bool add_tone(double freq, unsigned int duration)
{
  if (toneCount < maxTones) // buffer message
  {
    tones[toneCount].freq = freq;
    tones[toneCount].duration = duration;
    toneCount++;
    return (true);
  }
  else
    return (false); // no space for buffering
}

// EOF