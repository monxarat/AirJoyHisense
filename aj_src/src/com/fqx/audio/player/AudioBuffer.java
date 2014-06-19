package com.fqx.audio.player;

import java.util.concurrent.ConcurrentLinkedQueue;

public class AudioBuffer {
  ConcurrentLinkedQueue<byte[]> mAudioQueue = new ConcurrentLinkedQueue();

  public boolean add(byte[] paramArrayOfByte) {
    return this.mAudioQueue.add(paramArrayOfByte);
  }

  public void clear() {
    this.mAudioQueue.clear();
  }

  public byte[] get() {
    return (byte[])this.mAudioQueue.poll();
  }
}

