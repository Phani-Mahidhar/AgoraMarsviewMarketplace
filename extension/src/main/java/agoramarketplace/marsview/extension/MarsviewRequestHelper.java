package agoramarketplace.marsview.extension;

import android.os.AsyncTask;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class MarsviewRequestHelper {
    private String apiKey;
    private String apiSecret;
    private String userId;
    private String projectId;
    public MarsviewRequestHelper(String apiKey, String apiSecret, String userId, String projectId){
        this.apiKey = apiKey;
        this.apiSecret = apiSecret;
        this.userId = userId;
        this.projectId = projectId;
    }

    private JSONObject getAccessToken(){
        PostTask postTask = new PostTask();
        JSONObject outJson;
        String out;
        try{
            JSONObject object = new JSONObject();
            object.put("apiKey", this.apiKey);
            object.put("apiSecret", this.apiSecret);
            object.put("userId", this.userId);
            out = postTask.execute("https://api.marsview.ai/cb/v1/auth/get_access_token", object.toString()).get();
            return new JSONObject(out);
        }catch(Exception err){
            Log.e("Agora_Marsview_Java", err.toString());
            return null;
        }
    }

    public String postComputeDataRequest(String txnId, JSONArray enableModels){
        JSONObject accessTokenResponse = getAccessToken();
        String accessToken = null;
        try{
            if(accessTokenResponse.getBoolean("status")){
                JSONObject data = accessTokenResponse.getJSONObject("data");
                accessToken = data.getString("accessToken");
                JSONObject postData = new JSONObject();
                postData.put("userId", this.userId);
                postData.put("txnId", txnId);
                postData.put("enableModels",enableModels);
                String url = "https://api.marsview.ai/cb/v1/conversation/" + this.userId + "/compute";
                String postComputeDataResponse = new PostTask().execute(url, accessToken, postData.toString()).get();

                return postComputeDataResponse;
            }
            else{
                return accessTokenResponse.getString("err");
            }
        }catch(Exception e){
            Log.e("Agora_Marsview_Java", e.toString());
            return null;
        }

    }

    public String getProcessingState(String txnId){
        JSONObject accessTokenResponse = getAccessToken();
        String accessToken = null;
        try{
            if(accessTokenResponse.getBoolean("status")){
                JSONObject data = accessTokenResponse.getJSONObject("data");
                accessToken = data.getString("accessToken");
                JSONObject postData = new JSONObject();
                postData.put("userId", this.userId);
                postData.put("txnId", txnId);
                String url = "https://api.marsview.ai/cb/v1/conversation/get_txn/" + this.userId + "/" + txnId;
                String getProcessingStateResponse = new GetTask().execute(url, accessToken, postData.toString()).get();
                return getProcessingStateResponse;
            }
            else{
                return accessTokenResponse.getString("err");
            }
        }catch(Exception e){
            Log.e("Agora_Marsview_Java", e.toString());
            return null;
        }
    }

    public String getRequestMetadata(String txnId){
        JSONObject accessTokenResponse = getAccessToken();
        String accessToken = null;
        try{
            if(accessTokenResponse.getBoolean("status")){
                JSONObject data = accessTokenResponse.getJSONObject("data");
                accessToken = data.getString("accessToken");
                JSONObject postData = new JSONObject();
                postData.put("userId", this.userId);
                postData.put("txnId", txnId);
                String url = "https://api.marsview.ai/cb/v1/conversation/fetch_metadata/" + this.userId + "/" + txnId;
                String getRequestMetadataResponse = new GetTask().execute(url, accessToken, postData.toString()).get();
                return getRequestMetadataResponse;
            }
            else{
                return accessTokenResponse.getString("err");
            }
        }catch(Exception e){
            Log.e("Agora_Marsview_Java", e.toString());
            return null;
        }
    }


    private class PostTask extends AsyncTask<String, String, String> {
        @Override
        protected String doInBackground(String... strings) {
            HttpURLConnection conn = null;
            String urlString = strings[0];
            try{
                switch(strings.length){
                    case 2:{
                        String postData = strings[1];
                        URL url = new URL(urlString);
                        conn = (HttpURLConnection) url.openConnection();
                        conn.setRequestMethod("POST");
                        conn.setRequestProperty("Content-Type", "application/json");
                        conn.setRequestProperty("Content-Language", "en-US");
                        conn.setDoOutput(true);
                        conn.setUseCaches(true);
                        DataOutputStream wr = new DataOutputStream(conn.getOutputStream());
                        wr.writeBytes(postData);
                        wr.close();
                        break;
                    }
                    case 3:{
                        String accessToken = strings[1];
                        String postData = strings[2];
                        URL url = new URL(urlString);
                        conn = (HttpURLConnection) url.openConnection();
                        conn.setRequestMethod("POST");
                        conn.setRequestProperty("Content-Type", "application/json");
                        conn.setRequestProperty("Content-Language", "en-US");
                        conn.setRequestProperty("Authorization", accessToken);
                        conn.setDoOutput(true);
                        conn.setUseCaches(true);
                        DataOutputStream wr = new DataOutputStream(conn.getOutputStream());
                        wr.writeBytes(postData);
                        wr.close();
                        break;
                    }
                }
                InputStream is = conn.getInputStream();
                BufferedReader rd = new BufferedReader((new InputStreamReader(is)));
                StringBuilder response = new StringBuilder();
                String line;
                while((line = rd.readLine()) != null) {
                    response.append(line);
                    response.append('\r');
                }
                rd.close();
                return response.toString();
            } catch(Exception e){
                Log.e("Agora_Marsview_Java", e.toString());
                return null;
            } finally {
                if(conn != null){
                    conn.disconnect();
                }
            }
        }

        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);
            //Do anything with response..
        }
    }

    private class GetTask extends AsyncTask<String, String, String> {
        @Override
        protected String doInBackground(String... strings) {
            HttpURLConnection conn = null;
            String urlString = strings[0];
            try{
                URL url = new URL(urlString);
                conn = (HttpURLConnection) url.openConnection();
                if(strings.length >= 2){
                    String accessToken = strings[1];
                    conn.setRequestMethod("GET");
                    conn.setRequestProperty("Authorization", accessToken);
                }
                int responseCode = conn.getResponseCode();
                if(responseCode == HttpURLConnection.HTTP_OK){
                    InputStream is = conn.getInputStream();
                    BufferedReader rd = new BufferedReader((new InputStreamReader(is)));
                    StringBuilder response = new StringBuilder();
                    String line;
                    while((line = rd.readLine()) != null) {
                        response.append(line);
                        response.append('\r');
                    }
                    rd.close();
                    return response.toString();
                }
                else{
                    return null;
                }
            } catch(Exception e){
                Log.e("Agora_Marsview_Java", e.toString());
                return null;
            } finally {
                if(conn != null){
                    conn.disconnect();
                }
            }
        }

        @Override
        protected void onPostExecute(String result) {
            super.onPostExecute(result);
            //Do anything with response..
        }
    }

}


