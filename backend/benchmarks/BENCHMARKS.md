# Benchmark Results Summary

## 1. Pre-Parallelization (Single Core)

### Dispatcher Separate Style
- **European Options** (1,000,000 options):  
  ⏱ **179.404 ms**
- **American Options** (10,000 options):  
  ⏱ **63,436.1 ms**
- **Total Time**:  
  ⏱ **~63,615.5 ms**

---

### Dispatcher Mixed Style
- **European + American Options**:  
  ⏱ **63,267.8 ms**

---

### Black-Scholes SIMD (European Only)
- **1,000,000 Options**:  
  ⏱ **125.724 ms**

---

## 2. With OpenMP Enabled

### Dispatcher Mixed Style
- **European + American Options**:  
  ⏱ **3,183.96 ms**

---

## 3. With Full Parallelization

### Dispatcher Mixed Style (Parallelized)
- **European + American Options**:  
  ⏱ **405.129 ms**

---

### Dispatcher (Parallelized) – Only European
- **1,000,000 Options**:  
  ⏱ **17.7403 ms**

---

### Dispatcher (Parallelized) – Only American
- **10,000 Options**:  
  ⏱ **404.089 ms**

---

### Black-Scholes SIMD (Parallelized) - Only European
- **1,000,000 European Options**:  
  ⏱ **15.8119 ms**

