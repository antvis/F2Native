package com.antgroup.antv.f2;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 * @author qingyuan.yl
 * @date 2020-09-18
 */
public final class F2Config {


    private JSONObject mConfig;

    private F2Config(JSONObject cfg) {
        mConfig = cfg;
    }

    public String getStringField(String key) {
        try {
            return mConfig.getString(key);
        } catch (Exception e) {
            return null;
        }
    }

    public boolean getBoolField(String key, boolean def) {
        try {
            return mConfig.getBoolean(key);
        } catch (Exception e) {
            return def;
        }
    }

    String toJsonString() {
        if (mConfig == null) {
            return "{}";
        } else {
            return mConfig.toString();
        }
    }

    public static class Builder<T extends Builder> {
        protected JSONObject options = new JSONObject();

        public T setOption(String key, String value) {
            try {
                options.put(key, value);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, boolean value) {
            try {
                options.put(key, value);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, float value) {
            try {
                options.put(key, value);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, int value) {
            try {
                options.put(key, value);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, double value) {
            try {
                options.put(key, value);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, Object value) {
            try {
                options.put(key, value);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, String[] array) {
            try {
                JSONArray jsonArray = new JSONArray();
                for (int i = 0; i < array.length; i++) {
                    jsonArray.put(array[i]);
                }
                options.put(key, jsonArray);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, double[] array) {
            try {
                JSONArray jsonArray = new JSONArray();
                for (int i = 0; i < array.length; i++) {
                    jsonArray.put(array[i]);
                }
                options.put(key, jsonArray);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, JSONArray value) {
            try {
                options.put(key, value);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, F2Chart host, F2Function function) {
            try {
                function.bindChart(host);
                options.put(key, function.functionId);
            } catch (Exception e) {
            }
            return (T) this;
        }


        public F2Config build() {
            return new F2Config(options);
        }
    }
}
