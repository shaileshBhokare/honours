package org.smarteda.bus;

import java.io.Serializable;
import java.util.List;

public class ProductManager implements Serializable {

    private List products;

    public void setProducts(List p) {
        products = p;
    }

    public List getProducts() {
        return products;
    }
}
