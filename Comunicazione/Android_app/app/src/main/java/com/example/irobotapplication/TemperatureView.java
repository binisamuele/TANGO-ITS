package com.example.irobotapplication;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.view.View;

public class TemperatureView extends View {
    private String temperature = "";
    private String humidity = "";
    private TextPaint temperatureLabelPaint;
    private TextPaint humidityLabelPaint;
    private TextPaint valuePaint;

    public TemperatureView(Context context) {
        super(context);
        init();
    }

    public TemperatureView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public TemperatureView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        setBackgroundColor(Color.TRANSPARENT);

        temperatureLabelPaint = new TextPaint(Paint.ANTI_ALIAS_FLAG);
        temperatureLabelPaint.setColor(Color.WHITE);
        temperatureLabelPaint.setTextSize(50);

        humidityLabelPaint = new TextPaint(Paint.ANTI_ALIAS_FLAG);
        humidityLabelPaint.setColor(Color.WHITE);
        humidityLabelPaint.setTextSize(50);

        valuePaint = new TextPaint(Paint.ANTI_ALIAS_FLAG);
        valuePaint.setColor(Color.WHITE);
        valuePaint.setTextSize(60);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        float temperatureTextY = getHeight() / 4 - 30;
        float humidityTextY = 3 * getHeight() / 4 - 30;
        float textHeight = temperatureLabelPaint.descent() - temperatureLabelPaint.ascent();

        // Disegna l'icona del termometro in orizzontale con dimensioni doppie
        Drawable thermometerIcon = getResources().getDrawable(R.drawable.baseline_device_thermostat_24);
        drawIcon(canvas, thermometerIcon, getWidth() / 2, temperatureTextY - 10, true, 2.0f);

        // Disegna il valore della temperatura
        float temperatureValueX = (getWidth() - valuePaint.measureText(temperature)) / 2;
        float temperatureValueY = temperatureTextY + textHeight + 20;
        canvas.drawText(temperature, temperatureValueX, temperatureValueY, valuePaint);

        // Disegna l'icona della goccia d'acqua con dimensioni ridotte e alzata di qualche pixel
        Drawable waterDropIcon = getResources().getDrawable(R.drawable.baseline_water_drop_24);
        drawIcon(canvas, waterDropIcon, getWidth() / 2, humidityTextY - 5, false, 1.5f); // Riduci le dimensioni a 1.5x

        // Disegna il valore dell'umidità
        float humidityValueX = (getWidth() - valuePaint.measureText(humidity)) / 2;
        float humidityValueY = humidityTextY + textHeight + 20;
        canvas.drawText(humidity, humidityValueX, humidityValueY, valuePaint);
    }

    private void drawIcon(Canvas canvas, Drawable icon, float centerX, float centerY, boolean rotate, float scaleFactor) {
        if (icon != null) {
            if (rotate) {
                canvas.save();
                canvas.rotate(90, centerX, centerY);
            }

            int iconWidth = (int) (icon.getIntrinsicWidth() * scaleFactor);
            int iconHeight = (int) (icon.getIntrinsicHeight() * scaleFactor);

            int left = (int) (centerX - iconWidth / 2);
            int top = (int) (centerY - iconHeight / 2) - 15; // Alza di 15 pixel
            int right = left + iconWidth;
            int bottom = top + iconHeight;

            icon.setBounds(left, top, right, bottom);
            icon.draw(canvas);

            if (rotate) {
                canvas.restore();
            }
        }
    }





    public void setTemperature(String temperature) {
        this.temperature = temperature;
        invalidate();
    }

    public void setHumidity(String humidity) {
        this.humidity = humidity;
        invalidate();
    }
}
