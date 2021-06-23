package com.antgroup.antv.f2;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 * @author qingyuan.yl
 */
public final class F2Config {

    protected JSONObject mConfig;
    protected JSONArray mArrayConfig;

    private F2Config(JSONObject cfg) {
        mConfig = cfg;
    }

    private F2Config(JSONArray cfg) {
        mArrayConfig = cfg;
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

    public String toJsonString() {
        if (mArrayConfig != null) {
            return mArrayConfig.toString();
        } else if (mConfig == null) {
            return "{}";
        } else {
            return mConfig.toString();
        }
    }

    public static class Builder<T extends Builder> {
        protected JSONObject options = new JSONObject();
        protected JSONArray mArrayOptions = null;

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

        public T setOption(String key, F2Util.ColorGradient g) {
            return setOption(key, g.gradient.build().mConfig);
        }

        public T setOption(String key, F2Util.ColorGradient[] colors) {
            JSONArray array = new JSONArray();
            for (int i = 0; i < colors.length; i++) {
                array.put(colors[i].gradient.build().mConfig);
            }
            return setOption(key, array);
        }

        public T setOption(String key, F2Config.Builder config) {
            try {
                options.put(key, config.build().mConfig);
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
                for (String s : array) {
                    jsonArray.put(s);
                }
                options.put(key, jsonArray);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, int[] array) {
            try {
                JSONArray jsonArray = new JSONArray();
                for (int s : array) {
                    jsonArray.put(s);
                }
                options.put(key, jsonArray);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, float[] array) {
            try {
                JSONArray jsonArray = new JSONArray();
                for (double v : array) {
                    jsonArray.put(v);
                }
                options.put(key, jsonArray);
            } catch (Exception e) {
            }
            return (T) this;
        }

        public T setOption(String key, double[] array) {
            try {
                JSONArray jsonArray = new JSONArray();
                for (double v : array) {
                    jsonArray.put(v);
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

        public T putOption(Object item) {
            try {
                if (mArrayOptions == null) {
                    mArrayOptions = new JSONArray();
                }
                mArrayOptions.put(item);
            } catch (Exception e) {
            }
            return (T)this;
        }


        public F2Config build() {
            if (mArrayOptions != null) {
                return new F2Config(mArrayOptions);
            }
            return new F2Config(options);
        }
    }
}
