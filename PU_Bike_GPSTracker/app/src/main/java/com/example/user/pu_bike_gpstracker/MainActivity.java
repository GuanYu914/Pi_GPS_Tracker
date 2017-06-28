package com.example.user.pu_bike_gpstracker;

import android.os.AsyncTask;
import android.os.Handler;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;

import static com.example.user.pu_bike_gpstracker.R.id.map;

public class MainActivity extends FragmentActivity implements OnMapReadyCallback
{

    private GoogleMap                  PU_Bike_Map;
    private Handler                    HttpReq_Timer;
    private String                     TAG = MainActivity.class.getSimpleName();
    private static String              url = "http://140.128.13.39/App/db_view.php";
    private static String              No_CAR;
    private static double              lat;
    private static double              lon;

    private final int                  interval = 1000; //1000ms = 1s

    ArrayList<HashMap<String, String>> GPS_Info_List;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        GPS_Info_List = new ArrayList<>();
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager().findFragmentById(map);
        mapFragment.getMapAsync(this);
    }

    @Override
    public void onMapReady(GoogleMap googleMap)
    {
        PU_Bike_Map = googleMap;

        HttpReq_Timer = new Handler();
        final Runnable runnable = new Runnable()
        {
            @Override
            public void run()
            {
                Log.d(TAG, "run was called.");
                new HttpReq().execute();
                LatLng pos = new LatLng(lat, lon);
                PU_Bike_Map.addMarker(new MarkerOptions().position(pos).title(No_CAR+"'s position."));
                PU_Bike_Map.moveCamera(CameraUpdateFactory.newLatLng(pos));
                HttpReq_Timer.postDelayed(this, interval);
            }
        };

        HttpReq_Timer.postDelayed(runnable, interval);
    }


    class HttpReq extends AsyncTask<Void, Void, Void>
    {
        @Override
        protected void onPreExecute()
        {
            super.onPreExecute();
        }
        @Override
        protected Void doInBackground(Void... params)
        {
            HttpHandler sh = new HttpHandler();

            String jsonStr = sh.makeServiceCall(url);

            if(jsonStr != null)
            {
                try
                {
                    JSONObject jsonObject = new JSONObject(jsonStr);
                    JSONArray GPS_Info_Arr = jsonObject.getJSONArray("GPS_Info");

                    for(int i=0; i< GPS_Info_Arr.length(); i++)
                    {
                        JSONObject object = GPS_Info_Arr.getJSONObject(i);

                        String No_CAR = object.getString("No_CAR");
                        String Latitude = object.getString("Latitude");
                        String Longitude = object.getString("Longitude");

                        HashMap<String, String> GPS_Info = new HashMap<>();

                        GPS_Info.put("No_CAR", No_CAR);
                        GPS_Info.put("Latitude", Latitude);
                        GPS_Info.put("Longitude", Longitude);

                        GPS_Info_List.add(0 ,GPS_Info);
                    }
                }
                catch (final JSONException e)
                {
                    Log.e(TAG, "Json parsing error: " + e.getMessage());
                    runOnUiThread(new Runnable()
                    {
                        @Override
                        public void run()
                        {
                            Toast.makeText(getApplicationContext(), "Json parsing error: " + e.getMessage(), Toast.LENGTH_LONG).show();
                        }
                    });
                }
            }
            else
            {
                Log.e(TAG, "Couldn't get json from server.");
                runOnUiThread(new Runnable()
                {
                    @Override
                    public void run() {
                        Toast.makeText(getApplicationContext(), "Couldn't get json from server. Check LogCat for possible",Toast.LENGTH_SHORT).show();
                    }
                });
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result)
        {
            super.onPostExecute(result);
            No_CAR = GPS_Info_List.get(0).get("No_CAR");
            lat = Double.parseDouble(GPS_Info_List.get(0).get("Latitude"));
            lon = Double.parseDouble(GPS_Info_List.get(0).get("Longitude"));
        }
    }
}
