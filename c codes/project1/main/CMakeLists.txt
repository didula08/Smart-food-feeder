idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES relay svm ut  ut2 lcd_i2c nvs_flash esp_netif esp_http_client  esp_wifi
                       EMBED_TXTFILES "firebase_cert.pem" )
