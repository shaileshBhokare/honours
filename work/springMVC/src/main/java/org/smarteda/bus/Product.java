package org.smarteda.bus;


import java.io.Serializable;

public class Product implements Serializable {

    private String description;
    private Double price;

    public void setDescription(String s) {
        description = s;
    }

    public String getDescription() {
        return description;
    }

    public void setPrice(Double d) {
        price = d;
    }

    public Double getPrice() {
        return price;
    }

}