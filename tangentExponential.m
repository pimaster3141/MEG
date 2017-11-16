function out = tangentExponential(numTangents, lowTime, highTime)
    maxTime = highTime - lowTime;
    values = 0:maxTime;
    fcn = 128.^(values/maxTime) - 1;
    dfcn = 128.^(values./maxTime) .* log(128)./maxTime;
    cutoffs = linspace(dfcn(1), dfcn(end), numTangents+1);
    
    for p = 1:length(cutoffs)
        cutoffs(p) = find(dfcn >= cutoffs(p),1);
    end
    
    slopes = zeros(1, numTangents);
    for p = 1:length(slopes)
        slopes(p) = round((cutoffs(p+1) - cutoffs(p))./(fcn(cutoffs(p+1)) - fcn(cutoffs(p))))
    end
    
    intercepts = zeros(1, numTangents);
    for p = 2:length(intercepts);
        intercepts(p) = floor((cutoffs(p)- cutoffs(p-1))/slopes(p-1)) + intercepts(p-1);
    end
    
    out.cutoffs = cutoffs(1:end-1) - 1;
    out.slopes = slopes;
    out.intercepts = intercepts;
end
