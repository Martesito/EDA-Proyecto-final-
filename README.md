# EDA_PF_RestrepoAlejandro1 - Análisis de Redes de Transporte

## Descripción

Análisis completo de la red de carreteras de Pennsylvania (dataset SNAP) usando algoritmos de teoría de grafos implementados en C++.

El proyecto implementa:
- **Módulo A**: Análisis estructural (grado, componentes conexas, diámetro)
- **Módulo B**: Dijkstra con heap y BFS para 10 consultas punto a punto
- **Módulo C**: Extracción de subgrafo inducido, MST (Kruskal) y verificación de DAG

## Dataset

- **Nombre**: roadNet-PA (Pennsylvania Road Network)
- **Fuente**: [SNAP - Stanford Network Analysis Project](https://snap.stanford.edu/data/roadNet-PA.html)
- **Tamaño**: ~30 MB (descomprimido)
- **Composición**: 1,088,092 nodos, 1,541,898 aristas no dirigidas
- **Descarga**: https://snap.stanford.edu/data/roadNet-PA.txt.gz

### Instrucciones para Descargar el Dataset

```bash
# En PowerShell o bash
cd data/
wget https://snap.stanford.edu/data/roadNet-PA.txt.gz
# O usando curl:
curl -O https://snap.stanford.edu/data/roadNet-PA.txt.gz

# Descomprimir
gunzip roadNet-PA.txt.gz

# Verificar
ls -la roadNet-PA.txt
```

## Compilación

### Requisitos
- Compilador C++ con soporte C++17 (g++, clang, MSVC)
- CMake (opcional) o línea de comandos

### Compilación por Línea de Comandos

```bash
# Windows (PowerShell o CMD)
g++ -std=c++17 -O2 -o eda_pf.exe src/main.cpp src/graph.cpp src/loader.cpp src/bfs.cpp src/dijkstra.cpp src/structural.cpp src/subgraph.cpp

# Linux/Mac
g++ -std=c++17 -O2 -o eda_pf src/main.cpp src/graph.cpp src/loader.cpp src/bfs.cpp src/dijkstra.cpp src/structural.cpp src/subgraph.cpp
```



### Verificar Compilación

```bash
# Windows (PowerShell)
.\eda_pf.exe data/roadNet-PA.txt A

# Windows (CMD)
eda_pf.exe data/roadNet-PA.txt A

# Linux/Mac
./eda_pf data/roadNet-PA.txt A
```

Debe completarse sin errores y mostrar información de carga.

## Ejecución

El programa se ejecuta con dos argumentos:
```
eda_pf <archivo_dataset> <modulo>
```

### Módulo A: Análisis Estructural

```bash
# Windows (PowerShell)
.\eda_pf.exe data/roadNet-PA.txt A

# Windows (CMD)
eda_pf.exe data/roadNet-PA.txt A

# Linux/Mac
./eda_pf data/roadNet-PA.txt A
```

**Salida**: `results/analisis_estructural.txt`

Incluye:
- Número de nodos y aristas
- Nodos en componente conexa principal
- Grado promedio
- Nodo de mayor grado
- Número de componentes
- Diámetro estimado

**Tiempo estimado**: ~500 ms

### Módulo B: Consultas Punto a Punto (Dijkstra vs BFS)

```bash
# Windows (PowerShell)
.\eda_pf.exe data/roadNet-PA.txt B

# Windows (CMD)
eda_pf.exe data/roadNet-PA.txt B

# Linux/Mac
./eda_pf data/roadNet-PA.txt B
```

**Salida**: `results/consultas_p2p.csv`

Ejecuta 10 consultas fijas:
- Q01: 1 → 500000
- Q02: 100 → 1000000
- ... (10 total)

CSV incluye:
- Distancia Dijkstra (ponderada)
- Saltos BFS (no ponderado)
- Nodos explorados por cada algoritmo
- Tiempos en milisegundos

**Tiempo estimado**: ~2500 ms (Dijkstra domina el tiempo)

### Módulo C: Subgrafo, MST y DAG

```bash
# Windows (PowerShell)
.\eda_pf.exe data/roadNet-PA.txt C

# Windows (CMD)
eda_pf.exe data/roadNet-PA.txt C

# Linux/Mac
./eda_pf data/roadNet-PA.txt C
```

**Salidas**:
- `results/subgrafo_caminos.txt`: Subgrafo inducido (formato SNAP)
- `results/analisis_subgrafo.txt`: Análisis (nodos, aristas, MST, DAG)

**Proceso**:
1. Reconstruye caminos de Q01 (1 → 500000) y Q06 (1 → 1087562)
2. Extrae subgrafo inducido por esos nodos
3. Calcula MST usando Kruskal
4. Verifica si es DAG

**Tiempo estimado**: ~50 ms



## Archivos de Resultado

### analisis_estructural.txt
Estadísticas del grafo:
```
Numero de nodos: 1088092
Numero de aristas (no dirigidas): 1541898
Nodos en la componente conexa principal: 1087562
Grado promedio: 2.831
...
```

### consultas_p2p.csv
Tabla de resultados de 10 consultas:
```csv
consulta,origen,destino,dist_dijkstra,saltos_bfs,nodos_dijkstra,nodos_bfs,t_dijkstra_ms,t_bfs_ms
Q01,1,500000,25340,120,450000,550000,234.56,156.78
...
```

### subgrafo_caminos.txt
Subgrafo en formato SNAP:
```
# Subgrafo inducido
1	45
45	234
...
```

### analisis_subgrafo.txt
Análisis del subgrafo:
```
Numero de nodos: 245
Numero de aristas: 387
Peso total del MST: 1352
Es DAG: NO
```

## Decisiones de Diseño

### 1. Lista de Adyacencia vs Matriz
- **Matriz**: Requeriría ~4.7 TB (prohibitivo)
- **Lista**: ~50-80 MB (viable)
- Decidimos lista de adyacencia

### 2. Pesos Asignados
- Uniformes aleatorios [1, 10]
- srand(42) para reproducibilidad
- Todos los equipos obtienen mismos resultados

### 3. Mapeo de Nodos
- IDs originales no son consecutivos
- Internamente usamos índices 0..N-1
- Mapeo bidireccional mantiene compatibilidad

### 4. Diámetro Estimado
- BFS solo desde nodo de mayor grado (O(V+E))
- No desde todos los nodos (prohibitivo)
- Heurística razonable: nodo central alcanza distancias máximas

## Verificación de Resultados

### Comparación con SNAP

| Métrica | SNAP | Obtenido |
|---------|------|----------|
| Nodos | 1,088,092 | (ejecutar Módulo A) |
| Aristas | 1,541,898 | (ejecutar Módulo A) |
| Componente principal | 1,087,562 | (ejecutar Módulo A) |
| Diámetro | 782 | (ejecutar Módulo A) |

### Reproducibilidad P2P
Todos los equipos deben obtener exactamente los mismos resultados en Q01-Q10 si usan srand(42).

## Complejidad Teórica

| Componente | Complejidad | Justificación |
|-----------|------------|-----------------|
| Carga | O(V + E) | Parseo línea por línea |
| BFS | O(V + E) | Cola + lista de adyacencia |
| Dijkstra | O((V+E) log V) | Heap insert/extract |
| Componentes (DFS) | O(V + E) | Recorrido completo |
| Kruskal MST | O(E log E) | Ordenamiento de aristas |

## Notas Importantes

1. **Espacio en disco**: El dataset ocupa ~30 MB. Si hay limitaciones de GitHub, considerar Git LFS.

2. **Tiempos**: El programa puede tardar varios segundos. Es normal para 1M nodos.

3. **Pesos**: Los pesos son generados en memoria, no guardados. Cada ejecución con srand(42) obtiene los mismos pesos.

4. **Reproducibilidad**: Compilar con `-O2` es esencial para tiempos comparables.

5. **Manejo de memoria**: La lista de adyacencia ocupa ~50-80 MB. Máquinas modernas soportan fácilmente.

## Referencias

- Leskovec, J. & Krevl, A. (2014). SNAP Datasets: Stanford Large Network Dataset Collection. http://snap.stanford.edu/data


## Autor(es)

- Alejandro Restrepo

