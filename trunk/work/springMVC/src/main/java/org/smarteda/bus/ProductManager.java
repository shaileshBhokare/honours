package org.smarteda.bus;

import java.io.Serializable;
import java.util.List;
import java.util.ListIterator;

public class ProductManager implements Serializable {

    private List products;

    public void setProducts(List p) {
        products = p;
    }

    public List getProducts() {
        return products;
    }

    public void increasePrice(int pct) {
        ListIterator li = products.listIterator();
        while (li.hasNext()) {
            Product p = (Product) li.next();
            double newPrice = p.getPrice().doubleValue() * (100 + pct)/100;
            p.setPrice(new Double(newPrice));
        }
    }
}
