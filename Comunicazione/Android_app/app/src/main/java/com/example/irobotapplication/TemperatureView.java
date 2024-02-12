package com.example.irobotapplication;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
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

        float temperatureTextY = getHeight() / 4 - 30; // Ancora più in alto

        float humidityTextY = 3 * getHeight() / 4 - 30; // Ancora più in alto

        float textHeight = temperatureLabelPaint.descent() - temperatureLabelPaint.ascent();

        // Disegna "Temperatura"
        float temperatureTextX = (getWidth() - temperatureLabelPaint.measureText("Temperatura")) / 2;
        canvas.drawText("Temperatura", temperatureTextX, temperatureTextY, temperatureLabelPaint);

        // Cambia il colore del testo per i valori
        valuePaint.setColor(Color.WHITE);

        // Disegna il valore della temperatura
        float temperatureValueX = (getWidth() - valuePaint.measureText(temperature)) / 2;
        float temperatureValueY = temperatureTextY + textHeight + 20;
        canvas.drawText(temperature, temperatureValueX, temperatureValueY, valuePaint);

        // Disegna "Umidità"
        float humidityTextX = (getWidth() - humidityLabelPaint.measureText("Umidità")) / 2;
        canvas.drawText("Umidità", humidityTextX, humidityTextY, humidityLabelPaint);

        // Cambia il colore del testo per i valori
        valuePaint.setColor(Color.WHITE);

        // Disegna il valore dell'umidità
        float humidityValueX = (getWidth() - valuePaint.measureText(humidity)) / 2;
        float humidityValueY = humidityTextY + textHeight + 20;
        canvas.drawText(humidity, humidityValueX, humidityValueY, valuePaint);
    }

    // Metodi per impostare i valori della temperatura e dell'umidità
    public void setTemperature(String temperature) {
        this.temperature = temperature;
        invalidate(); // Forza la ridisegnazione della vista
    }

    public void setHumidity(String humidity) {
        this.humidity = humidity;
        invalidate(); // Forza la ridisegnazione della vista
    }
}
