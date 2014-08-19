package com.fqx.anyplay.images;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.RectF;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.widget.ImageView;

public class MyImageView extends ImageView {
	@SuppressWarnings("unused")
	private static final String TAG = "ImageViewTouchBase";
	
	private int screenWidth;
	private int screenHeight;
	
	public  boolean doubleUpflag = false;
	// This is the base transformation which is used to show the image
	// initially. The current computation for this shows the image in
	// it's entirety, letterboxing as needed. One could choose to
	// show the image as cropped instead.
	//
	// This matrix is recomputed when we go from the thumbnail image to
	// the full size image.
	protected Matrix mBaseMatrix = new Matrix();

	// This is the supplementary transformation which reflects what
	// the user has done in terms of zooming and panning.
	//
	// This matrix reImagesActivitys the same when we go from the thumbnail image
	// to the full size image.
	protected Matrix mSuppMatrix = new Matrix();

	// This is the final matrix which is computed as the concatentation
	// of the base matrix and the supplementary matrix.
	private final Matrix mDisplayMatrix = new Matrix();

	// Temporary buffer used for getting the values out of a matrix.
	private final float[] mMatrixValues = new float[9];

	// The current bitmap being displayed.
	// protected final RotateBitmap mBitmapDisplayed = new RotateBitmap(null);
	protected Bitmap image = null;

	int mThisWidth = -1, mThisHeight = -1;

	float mMaxZoom = 4.0f;//
	float mMinZoom = 1.0f;

	 int imageWidth;
	 int imageHeight;

	private float scaleRate;

	public MyImageView(Context context) {
		super(context);

		init();
	}

	public MyImageView(Context context, AttributeSet attrs) {
		super(context, attrs);

		init();
	}
	
	public void setScreenSize(int height ,int width){
		screenWidth =width;
		screenHeight = height;
	}
	
	public 	int originrate = 1;
	
	private float mLastRate = -100;
    /**
     * 缩放照片
     *  @photoId        [in] 标识照片的唯一字符串
     *  @rate           [in] 缩放比例
     *                       rate = 0: 缩放至最合适大小
     *                       rate = 1: 缩放至原大小
     *                       rate > 1: 放大
     *                       0 < rate < 1, 缩小
     *  @return
     *     ResultCodeOk - 成功
     *     ResultCodeErrorNotSupport - 不支持此功能
     */
	public void zoomPhoto(float rate){
		if((rate == mLastRate) || (image == null)) {
			return;
		}
		mLastRate = rate;
		if(rate == 0){ 
			zoomTo(getScaleRate(),  screenWidth / 2,  screenHeight / 2, 200f);
		}else if(rate ==1){
			zoomTo(1,  screenWidth / 2,  screenHeight / 2, 200f);
		}else if(rate>0){
			float cx = getWidth() / 2F;
			float cy = getHeight() / 2F;
			mSuppMatrix.postScale(rate, rate, cx, cy);
			setImageMatrix(getImageViewMatrix());
			center(true, true);
		}
	}
	
	public void translatePhoto(float rateX ,float rateY){
		float x = rateX*getInitX();
		float y = rateY*getInitY() ;
		postTranslate(x,y);
	}
	
	
	public void rotatePhoto( int direction){
//		DebugUtil.debug("rotatePhoto", "position= "+position);
//		int x = (direction+4-position)%4;
//		if(x%2==1){
//			rotateinit();
//		}
//		
//		Log.d("MyImageView", "rotatePhoto direction ="+direction+" x = "+x+" postion = "+position);
//		mSuppMatrix.postRotate(-90*x, screenWidth / 2, screenHeight / 2);
//		System.out.println();
//		DebugUtil.debug("rotatePhoto", "x= "+x);
//		setImageMatrix(mSuppMatrix);
//		
//		
//		position = direction;
		
		
		if (direction == 1) {
			zoomright();
		}else if (direction ==3) {
			zoomleft();
		}
		
	}
	
	
	
	private  float 	initX;
	private  float 	initY;
	
	public float getInitX() {
		return initX;
	}

	public float getInitY() {
		return initY;
	}

	public void initXY(){

//		initX = getScale() * getImageWidth();
//		initY = getScale()
//				* getImageHeight();
		initX = screenWidth;
		initY =	screenHeight;
	}
	
	
	
	
	
	
	/**
	 * 
	 * @param rate
	 */
	public void zoomPhoto(float rate ,float distanceX,float distanceY){
		if(rate == 0){
			  
		}else{
			
		}
	}

	
	public void rotateinit(){
		imageWidth = imageWidth + imageHeight;
		imageHeight= imageWidth - imageHeight;
		imageWidth = imageWidth - imageHeight;
		mMaxZoom = 4.0f;
		mMinZoom = 1.0f;
		arithScaleRate();
		getScale();
		System.out.println("getScale()"+getScale());
		if(scaleRate>4.0){
			mMaxZoom=scaleRate;
		}else{
			mMaxZoom=4.0f;
		}
		
		mMinZoom = (screenWidth / 1f) / imageWidth;
		if(mMinZoom>1.0f){
			mMinZoom=1.0f;
		}
	}
	

	
	

    int position = 0;
    
    public int getPosition() {
		return position;
	}
	public void setPosition(int position) {
		this.position = position;
	}

   

	private void arithScaleRate() {
		float scaleWidth = screenWidth / (float) imageWidth;
		float scaleHeight = screenHeight / (float) imageHeight;
		scaleRate = Math.min(scaleWidth, scaleHeight);
	}

	public float getScaleRate() {
		return scaleRate;
	}

	public int getImageWidth() {
		return imageWidth;
	}

	public void setImageWidth(int imageWidth) {
		this.imageWidth = imageWidth;
	}

	public int getImageHeight() {
		return imageHeight;
	}

	public void setImageHeight(int imageHeight) {
		this.imageHeight = imageHeight;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {
			event.startTracking();
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK && event.isTracking()
				&& !event.isCanceled()) {
			if (getScale() > 1.0f) {
				// If we're zoomed in, pressing Back jumps out to show the
				// entire image, otherwise Back returns the user to the gallery.
				zoomTo(1.0f);
				return true;
			}
		}
		return super.onKeyUp(keyCode, event);
	}

	protected Handler mHandler = new Handler();

	@Override
	public void setImageBitmap(Bitmap bitmap) {
		super.setImageBitmap(bitmap);
		image = bitmap;
		this.imageHeight = image.getHeight();
		this.imageWidth = image.getWidth();
		arithScaleRate();

		if(scaleRate>4.0){
			mMaxZoom=scaleRate;
		}else{
			mMaxZoom=4.0f;
		}
		
		mMinZoom = (screenWidth / 1f) / imageWidth;
		if(mMinZoom>1.0f){
			mMinZoom=1.0f;
		}
		
//		zoomTo(scaleRate, screenWidth / 2f, screenHeight / 2f);

		layoutToCenter();
		
		initXY();

		// imageView.zoomTo(scaleRate, screenWidth / 2,
		// screenHeight / 2
		center(true, true);
	}

	// Center as much as possible in one or both axis. Centering is
	// defined as follows: if the image is scaled down below the
	// view's dimensions then center it (literally). If the image
	// is scaled larger than the view and is translated out of view
	// then translate it back into view (i.e. eliminate black bars).
	protected void center(boolean horizontal, boolean vertical) {
		// if (mBitmapDisplayed.getBitmap() == null) {
		// return;
		// }
		if (image == null) {
			return;
		}

		Matrix m = getImageViewMatrix();

		RectF rect = new RectF(0, 0, image.getWidth(), image.getHeight());
		// RectF rect = new RectF(0, 0, imageWidth*getScale(),
		// imageHeight*getScale());

		m.mapRect(rect);

		float height = rect.height();
		float width = rect.width();

		float deltaX = 0, deltaY = 0;

		if (vertical) {
			int viewHeight = getHeight();
			if (height < viewHeight) {
				deltaY = (viewHeight - height) / 2 - rect.top;
			} else if (rect.top > 0) {
				deltaY = -rect.top;
			} else if (rect.bottom < viewHeight) {
				deltaY = getHeight() - rect.bottom;
			}
		}

		if (horizontal) {
			int viewWidth = getWidth();
			if (width < viewWidth) {
				deltaX = (viewWidth - width) / 2 - rect.left;
			} else if (rect.left > 0) {

				deltaX = -rect.left;
			} else if (rect.right < viewWidth) {
				deltaX = viewWidth - rect.right;
			}
		}
		postTranslate(deltaX, deltaY);
		setImageMatrix(getImageViewMatrix());
	}

	private void init() {
		setScaleType(ImageView.ScaleType.MATRIX);
		
	}
	

	public void layoutToCenter() {
		float width = imageWidth * getScale();
		float height = imageHeight * getScale();
		float fill_width = screenWidth - width;
		float fill_height = screenHeight - height;
		float tran_width = 0f;
		float tran_height = 0f;
		if (fill_width > 0)
			tran_width = fill_width / 2;
		if (fill_height > 0)
			tran_height = fill_height / 2;

		postTranslate(tran_width, tran_height);
		setImageMatrix(getImageViewMatrix());
//		center(true, true);
	}

	protected float getValue(Matrix matrix, int whichValue) {
		matrix.getValues(mMatrixValues);
		
		return mMatrixValues[whichValue];
	}

	// Get the scale factor out of the matrix.
/**
 * 
 * @param matrix
 * @return
 */
	protected float getScale(Matrix matrix) {
		if(position%2==0){
			return Math.abs(getValue(matrix, Matrix.MSCALE_X));
		}else{
			return  Math.abs(getValue(matrix, Matrix.MSKEW_X));
		}
		
		
	}

	protected float getScale() {
		return getScale(mSuppMatrix);
	}

	// Combine the base matrix and the supp matrix to make the final matrix.
	protected Matrix getImageViewMatrix() {
		// The final matrix is computed as the concatentation of the base matrix
		// and the supplementary matrix.
		mDisplayMatrix.set(mBaseMatrix);
		mDisplayMatrix.postConcat(mSuppMatrix);
		
		return mDisplayMatrix;
	}

	static final float SCALE_RATE = 1.25F;

	// Sets the maximum zoom, which is a scale relative to the base matrix. It
	// is calculated to show the image at 400% zoom regardless of screen or
	// image orientation. If in the future we decode the full 3 megapixel image,
	// rather than the current 1024x768, this should be changed down to 200%.
	protected float maxZoom() {
		if (image == null) {
			return 1F;
		}

		float fw = (float) image.getWidth() / (float) mThisWidth;
		float fh = (float) image.getHeight() / (float) mThisHeight;
		float max = Math.max(fw, fh) * 4;
		return max;
	}

	protected float zoomTo(float scale, float centerX, float centerY) {
		
//		if (scale > mMaxZoom) {
//			scale = mMaxZoom;
//		} else if (scale < mMinZoom) {
//			scale = mMinZoom;
//		}

		float oldScale = getScale();
		float deltaScale = scale / oldScale;

		mSuppMatrix.postScale(deltaScale, deltaScale, centerX, centerY);
		setImageMatrix(getImageViewMatrix());
		center(true, true);
		return deltaScale;
	}

	protected float zoomTo(final float scale, final float centerX,
			final float centerY, final float durationMs) {
		final float incrementPerMs = (scale - getScale()) / durationMs;
		final float oldScale = getScale();
		final long startTime = System.currentTimeMillis();

		mHandler.post(new Runnable() {
			public void run() {
				long now = System.currentTimeMillis();
				float currentMs = Math.min(durationMs, now - startTime);
				float target = oldScale + (incrementPerMs * currentMs);
				zoomTo(target, centerX, centerY);
				if (currentMs < durationMs) {
					mHandler.post(this);
				}
			}
		});
		
		return scale/oldScale;
	}

	protected float zoomTo(float scale) {
		float cx = getWidth() / 2F;
		float cy = getHeight() / 2F;
		float deltaScale = zoomTo(scale, cx, cy);
		return deltaScale;
	}

	protected void zoomToPoint(float scale, float pointX, float pointY) {
		float cx = getWidth() / 2F;
		float cy = getHeight() / 2F;
		panBy(cx - pointX, cy - pointY);
		zoomTo(scale, cx, cy);
	}

	protected void zoomIn() {
		zoomIn(SCALE_RATE);
	}

	protected void zoomOut() {
		zoomOut(SCALE_RATE);
	}

	protected void zoomIn(float rate) {
		
		/**
		 * 
		 */
		
		float cx = getWidth() / 2F;
		float cy = getHeight() / 2F;
		if (image == null) {
			return;
		}

		mSuppMatrix.postScale(rate, rate, cx, cy);
		setImageMatrix(getImageViewMatrix());
		if (getScale() >= mMaxZoom) {
			zoomTo(mMaxZoom, cx, cy, 200);
		}
		
		
	}

	public void postTranslate(float dx, float dy) {
		mSuppMatrix.postTranslate(dx, dy);
		setImageMatrix(getImageViewMatrix());
	}

	protected void zoomOut(float rate) {
		if (image == null) {
			return;
		}

		float cx = getWidth() / 2F;
		float cy = getHeight() / 2F;

		System.out.println(mMinZoom+":mMinZoom");

		mSuppMatrix.postScale(1F / rate, 1F / rate, cx, cy);
		
		setImageMatrix(getImageViewMatrix());
		center(true, true);
		
		System.out.println("mMinZoom:"+mMinZoom+"=="+getScale());
		if(getScale()<mMinZoom){
			zoomTo(mMinZoom, cx, cy, 200);
		}
		
	}

	float _dy = 0.0f;

	protected void postTranslateDur(final float dy, final float durationMs) {
		_dy = 0.0f;
		final float incrementPerMs = dy / durationMs;
		final long startTime = System.currentTimeMillis();
		mHandler.post(new Runnable() {
			public void run() {
				long now = System.currentTimeMillis();
				float currentMs = Math.min(durationMs, now - startTime);

				postTranslate(0, incrementPerMs * currentMs - _dy);
				_dy = incrementPerMs * currentMs;

				if (currentMs < durationMs) {
					mHandler.post(this);
				}
			}
		});
	}

	protected void panBy(float dx, float dy) {
		postTranslate(dx, dy);
		setImageMatrix(getImageViewMatrix());
	}
	
	public void zoomright(){
//		zoomTo(getScaleRate(),screenWidth / 2,screenHeight / 2, 200f);
//		setImageMatrix(mSuppMatrix);
		imageWidth = imageWidth + imageHeight;
		imageHeight= imageWidth - imageHeight;
		imageWidth = imageWidth - imageHeight;
		mMaxZoom = 4.0f;
		mMinZoom = 1.0f;
		arithScaleRate();
/**
 */
		getScale();
		System.out.println("getScale()"+getScale());
		if(scaleRate>4.0){
			mMaxZoom=scaleRate;
		}else{
			mMaxZoom=4.0f;
		}
		
		
/**
 */
		mMinZoom = (screenWidth / 1f) / imageWidth;
		if(mMinZoom>1.0f){
			mMinZoom=1.0f;
		}
		
		mSuppMatrix.postRotate(90, screenWidth / 2, screenHeight / 2);
		System.out.println(getScale()+"=="+getImageWidth()+"=="+getHeight()+"=="+getRight());
		setImageMatrix(mSuppMatrix);
		
		position = (position+4-1)%4;
//		zoomTo(getScaleRate(), screenWidth / 2,
//				screenHeight / 2);
		
	}
	
	public  void zoomleft(){
//		zoomTo(getScaleRate(),screenWidth / 2,screenHeight / 2, 200f);
//		setImageMatrix(mSuppMatrix);
		imageWidth = imageWidth + imageHeight;
		imageHeight= imageWidth - imageHeight;
		imageWidth = imageWidth - imageHeight;
		mMaxZoom = 4.0f;
		mMinZoom = 1.0f;
		arithScaleRate();
		getScale();
		System.out.println("getScale()"+getScale());
		if(scaleRate>4.0){
			mMaxZoom=scaleRate;
		}else{
			mMaxZoom=4.0f;
		}
		
		mMinZoom = (screenWidth / 1f) / imageWidth;
		if(mMinZoom>1.0f){
			mMinZoom=1.0f;
		}
		
		mSuppMatrix.postRotate(-90, screenWidth / 2, screenHeight / 2);
		System.out.println(getScale()+"=="+getImageWidth()+"=="+getHeight()+"=="+getRight());
		setImageMatrix(mSuppMatrix);
		
		
		position = (position+1)%4;
//		
//		zoomTo(getScaleRate(), screenWidth / 2,
//				screenHeight / 2);
		
	}
	
}
