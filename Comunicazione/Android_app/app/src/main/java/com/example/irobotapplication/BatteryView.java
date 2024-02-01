package com.example.irobotapplication;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.PaintDrawable;
import android.util.AttributeSet;
import android.view.View;

public class BatteryView extends View {
    private float radius = 0f;
    private boolean isCharging = false;

    private PaintDrawable topPaint = new PaintDrawable(Color.WHITE);
    private Rect topRect = new Rect();
    private int topPaintWidthPercent = 50;
    private int topPaintHeightPercent = 8;

    // Border
    private Paint borderPaint = new Paint();
    private RectF borderRect = new RectF();
    private int borderStrokeWidthPercent = 8;
    private float borderStroke = 0f;

    // Percent
    private Paint percentPaint = new Paint();
    private RectF percentRect = new RectF();
    private float percentRectTopMin = 0f;
    private int percent = 0;

    // Charging
    private RectF chargingRect = new RectF();
    private Bitmap chargingBitmap;

    // Added text percentage
    private Paint textPaint = new Paint();
    private String percentageText = "";

    public BatteryView(Context context) {
        super(context);
        init(null);
    }

    public BatteryView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs);
    }

    public BatteryView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(attrs);
    }

    private void init(AttributeSet attrs) {
        TypedArray ta = getContext().obtainStyledAttributes(attrs, R.styleable.BatteryView);
        try {
            percent = ta.getInt(R.styleable.BatteryView_bv_percent, 0);
            isCharging = ta.getBoolean(R.styleable.BatteryView_bv_charging, false);
            // Added to initialize percentage text
            percentageText = percent + "%";
        } finally {
            ta.recycle();
        }
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int measureWidth = View.getDefaultSize(getSuggestedMinimumWidth(), widthMeasureSpec);
        int measureHeight = (int) (measureWidth * 1.8f);
        setMeasuredDimension(measureWidth, measureHeight);

        radius = borderStroke / 2;
        borderStroke = (borderStrokeWidthPercent * measureWidth) / 100;

        // Top
        float topLeft = measureWidth * ((100 - topPaintWidthPercent) / 2f) / 100f;
        float topRight = measureWidth - topLeft;
        float topBottom = topPaintHeightPercent * measureHeight / 100f;
        topRect.set((int) topLeft, 0, (int) topRight, (int) topBottom);

        // Border
        float borderLeft = borderStroke / 2f;
        float borderTop = topBottom + borderStroke / 2f;
        float borderRight = measureWidth - borderStroke / 2f;
        float borderBottom = measureHeight - borderStroke / 2f;
        borderRect.set(borderLeft, borderTop, borderRight, borderBottom);

        // Progress
        float progressLeft = borderStroke;
        percentRectTopMin = topBottom + borderStroke;
        float progressRight = measureWidth - borderStroke;
        float progressBottom = measureHeight - borderStroke;
        percentRect.set(progressLeft, percentRectTopMin, progressRight, progressBottom);

        // Charging Image
        float chargingLeft = borderStroke;
        float chargingTop = topBottom + borderStroke;
        float chargingRight = measureWidth - borderStroke;
        float chargingBottom = measureHeight - borderStroke;
        float diff = ((chargingBottom - chargingTop) - (chargingRight - chargingLeft));
        chargingTop += diff / 2;
        chargingBottom -= diff / 2;
        chargingRect.set(chargingLeft, chargingTop, chargingRight, chargingBottom);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        drawTop(canvas);
        drawBody(canvas);
        drawProgress(canvas, percent); // Added to display the percentage
        if (isCharging) {
            drawCharging(canvas);
        }
        drawPercentageText(canvas); // Added to display the percentage text
    }

    private void drawTop(Canvas canvas) {
        topPaint.setBounds(topRect);
        topPaint.setCornerRadii(new float[]{radius, radius, radius, radius, 0f, 0f, 0f, 0f});
        topPaint.draw(canvas);
    }

    private void drawBody(Canvas canvas) {
        borderPaint.setStrokeWidth(borderStroke);
        canvas.drawRoundRect(borderRect, radius, radius, borderPaint);
    }

    private void drawProgress(Canvas canvas, int percent) {
        percentPaint.setColor(getPercentColor(percent));
        percentRect.top = percentRectTopMin + (percentRect.bottom - percentRectTopMin) * (100 - percent) / 100;
        canvas.drawRect(percentRect, percentPaint);
    }

    private int getPercentColor(int percent) {
        if (percent > 50) {
            return Color.GREEN;
        }
        if (percent > 30) {
            return Color.YELLOW;
        }
        return Color.RED;
    }

    private void drawCharging(Canvas canvas) {
        if (chargingBitmap != null) {
            canvas.drawBitmap(chargingBitmap, null, chargingRect, null);
        }
    }

    private void drawPercentageText(Canvas canvas) {
        textPaint.setColor(Color.WHITE);
        textPaint.setTextSize(50);
        textPaint.setTextAlign(Paint.Align.CENTER);
        float textX = getWidth() / 2f;
        float textY = getHeight() / 2f - ((textPaint.descent() + textPaint.ascent()) / 2f);
        canvas.drawText(percentageText, textX, textY, textPaint);
    }

    public void charge() {
        isCharging = true;
        invalidate();
    }

    public void unCharge() {
        isCharging = false;
        invalidate();
    }

    public void setPercent(int percent) {
        if (percent > 100 || percent < 0) {
            return;
        }
        this.percent = percent;
        this.percentageText = percent + "%"; // Added to update the percentage text
        invalidate();
    }

    public int getPercent() {
        return percent;
    }
}
